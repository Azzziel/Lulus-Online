import 'dart:async';
import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:http/http.dart';
import 'package:geolocator/geolocator.dart';
import 'package:permission_handler/permission_handler.dart';

import 'post_handler.dart';

const SERVER_BASE_URL = 'http://www.onspot.my.id/api/mobile_app/';
const SERVER_AUTH_KEY = '98d11adf61a1fc9ab76ee375d0600170';

class NodeLocation {
  NodeLocation(this.id, this.name, this.latitude, this.longitude,
      this.googlePlaceId, this.space);

  int id;
  String name;
  double latitude;
  double longitude;
  String googlePlaceId;
  int space;
  double _distance; // Not initialized by the constructor
  double get distance => _distance; // No setter
}

class NodeLocationModel extends ChangeNotifier {
  NodeLocationModel() {
    _initialize();
  }

  PostHandler _postHandler = PostHandler(SERVER_BASE_URL, SERVER_AUTH_KEY);
  Geolocator _geolocator = Geolocator();

  List<NodeLocation> _nodeLocations = [];
  List<NodeLocation> get nodeLocations => _nodeLocations; // No setter

  Position _previousPosition;
  Position _position;
  Position get position => _position; // No setter

  Timer _timer;

//  PermissionStatus _permissionLocationStatus = PermissionStatus.undetermined;
//  PermissionStatus get permissionLocationStatus => _permissionLocationStatus;

  PermissionStatus _permissionLocationAlwaysStatus =
      PermissionStatus.undetermined;
  PermissionStatus get permissionLocationAlwaysStatus =>
      _permissionLocationAlwaysStatus;

//  PermissionStatus _permissionLocationWhenInUseStatus =
//      PermissionStatus.undetermined;
//  PermissionStatus get permissionLocationWhenInUseStatus =>
//      _permissionLocationWhenInUseStatus;

  Future<void> _checkLocationPermission() async {
//    _permissionLocationStatus = await Permission.location.status;
    _permissionLocationAlwaysStatus = await Permission.locationAlways.status;
//    _permissionLocationWhenInUseStatus =
//        await Permission.locationWhenInUse.status;
//    notifyListeners();

    if (_permissionLocationAlwaysStatus != PermissionStatus.granted) {
      requestLocationPermission();
    }
  }

  Future<void> requestLocationPermission() async {
    try {
      _permissionLocationAlwaysStatus =
          await Permission.locationAlways.request();
      notifyListeners();
    } catch (e) {
      print('[] Exception details: $e');
    }
  }

  int calculateClosestNodeLocationId() {
    int closestId;

    double smallest = double.maxFinite;
    for (var nodeLocation in nodeLocations) {
      if (nodeLocation.distance != null) {
        if (nodeLocation.distance < smallest) {
          smallest = nodeLocation.distance;
          closestId = nodeLocation.id;
        }
      } else {
        return null;
      }
    }

    return closestId;
  }

  // Mutual exclusion is applied here
  bool _isLoadingPositionAndDistance = false;
  Future<void> _loadPositionAndDistance() async {
    if (!_isLoadingPositionAndDistance) {
      _isLoadingPositionAndDistance = true;

      await _loadPosition();
      _loadNodeLocationDistances();

      _isLoadingPositionAndDistance = false;
    }
  }

  Future<void> _initialize() async {
    _checkLocationPermission();
    await _loadNodeLocations();

    _timer = Timer.periodic(Duration(milliseconds: 200), (timer) {
      _loadFreeSpaces();
      _loadPositionAndDistance();
    });
  }

  @override
  void dispose() {
    _timer?.cancel(); // Note the null-aware member access operator [?.]
    super.dispose();
  }

  Future<void> _loadNodeLocations() async {
    Response response = await _postHandler.getResponse('get_locations.php');

    if (response.statusCode == 200) {
      List<dynamic> jsonData = jsonDecode(response.body);

      jsonData.forEach((element) {
        nodeLocations.add(
          NodeLocation(
            int.parse(element['lc_id']),
            element['lc_name'],
            double.parse(element['lc_lat']),
            double.parse(element['lc_long']),
            element['google_place_id'],
            int.parse(element['free_space']),
          ),
        );
      });

      notifyListeners();
    }
  }

  // Mutual exclusion is applied here
  bool _isRunningLoadPosition = false;
  Future<void> _loadPosition() async {
    if (!_isRunningLoadPosition) {
      _isRunningLoadPosition = true;

      // TODO: Fix the bug caused by choosing when-in-use-only permission
      if (_permissionLocationAlwaysStatus.isGranted) {
        _position = await _geolocator.getCurrentPosition(
          desiredAccuracy: LocationAccuracy.bestForNavigation,
        );
      }

      notifyListeners();
      _isRunningLoadPosition = false;
    }
  }

  String _previousResponseBody;

  // Mutual exclusion is applied here
  bool _isRunningLoadFreeSpaces = false;
  Future<void> _loadFreeSpaces() async {
    if (!_isRunningLoadFreeSpaces) {
      _isRunningLoadFreeSpaces = true;

      Response response = await _postHandler.getResponse('get_free_spaces.php');

      if (response.statusCode == 200) {
        if (_previousResponseBody == response.body) {
          List<dynamic> jsonData = jsonDecode(response.body);

          for (var jsonRow in jsonData) {
            int index = nodeLocations.indexWhere(
                    (element) => element.id == int.parse(jsonRow['lc_id']));
            nodeLocations[index].space = int.parse(jsonRow['free_space']);
          }

          notifyListeners();
          _previousResponseBody = response.body;
        }
      }

      _isRunningLoadFreeSpaces = false;
    }
  }

  Future<void> _loadNodeLocationDistances() async {
    if (_position != null) {
      if (_previousPosition != _position) {
        nodeLocations.forEach((element) async {
          element._distance = await _geolocator.distanceBetween(
            _position.latitude,
            _position.longitude,
            element.latitude,
            element.longitude,
          );

          notifyListeners();
        });

        try {
          nodeLocations.sort((a, b) => a._distance.compareTo(b._distance));
          notifyListeners();
        } catch (e) {
          print('[] Exception details: $e');
        }

        _previousPosition = _position;
      }
    }
  }
}
