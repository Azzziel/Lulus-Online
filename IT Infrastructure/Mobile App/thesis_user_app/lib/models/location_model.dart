import 'dart:async';
import 'dart:convert';
import 'package:flutter/material.dart';
import 'package:http/http.dart';
import 'package:geolocator/geolocator.dart';
import 'package:permission_handler/permission_handler.dart';

import 'post_handler.dart';

const SERVER_URL = 'http://onspot.my.id/api/maps/';
const SERVER_API_KEY = '5cb4bf3e42ff76ca9186850b9017bdc8';

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

  GetHandler _httpHandler = GetHandler(SERVER_URL, SERVER_API_KEY);
  Geolocator _geolocator = Geolocator();

  List<NodeLocation> _nodeLocations = [];
  List<NodeLocation> get nodeLocations => _nodeLocations; // No setter

  Position _previousPosition;
  Position _position;
  Position get position => _position; // No setter

  Timer _timer;
  Duration _timerDuration = Duration(milliseconds: 200);
  void _timerCallback(Timer timer) {
    _loadFreeSpaces();
    _loadPositionAndDistance();
  }

  void startTimer() {
    _timer = Timer.periodic(_timerDuration, _timerCallback);
  }

  void cancelTimer() {
    _timer?.cancel(); // Notice the null-aware operator
  }

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

    startTimer();
  }

  @override
  void dispose() {
    cancelTimer();
    super.dispose();
  }

  Future<void> _loadNodeLocations() async {
    Response response = await _httpHandler.getResponse('location');

    if (response.statusCode == 200) {
      dynamic jsonData = jsonDecode(response.body);

      if (jsonData['status'] == 200) {
        dynamic jsonValues = jsonData['values'];

        for (int i = 0; i < jsonValues.length; ++i) {
          nodeLocations.add(
            NodeLocation(
              jsonValues[i]['lc_id'],
              jsonValues[i]['lc_name'],
              jsonValues[i]['lc_lat'],
              jsonValues[i]['lc_long'],
              jsonValues[i]['google_place_id'],
              jsonValues[i]['free_space'],
            ),
          );
        }

        notifyListeners();
      }
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

      Response response = await _httpHandler.getResponse('freespace');

      if (response.statusCode == 200) {
        if (_previousResponseBody == response.body) {
          dynamic jsonData = jsonDecode(response.body);

          if (jsonData['status'] == 200) {
            dynamic jsonValues = jsonData['values'];

            for (var value in jsonValues) {
              int index = nodeLocations
                  .indexWhere((element) => element.id == value['lc_id']);
              nodeLocations[index].space = value['free_space'];
            }

            notifyListeners();
          }

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
