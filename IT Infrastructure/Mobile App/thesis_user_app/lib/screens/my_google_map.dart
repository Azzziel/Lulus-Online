import 'dart:async';
import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'package:provider/provider.dart';
import 'package:permission_handler/permission_handler.dart';

import '../common/colors.dart';
import '../models/location_model.dart';
import 'location_detail_page.dart';

// Special Capital Region of Jakarta
const _DEFAULT_CAMERA_POSITION = CameraPosition(
  target: LatLng(-6.2088, 106.8456),
  zoom: 10.5,
);

const _DEFAULT_MAP_STYLE =
    '[{"featureType":"administrative.land_parcel","elementType":"labels","stylers":[{"visibility":"off"}]},{"featureType":"poi","elementType":"labels.text","stylers":[{"visibility":"off"}]},{"featureType":"poi.business","stylers":[{"visibility":"off"}]},{"featureType":"poi.park","elementType":"labels.text","stylers":[{"visibility":"off"}]},{"featureType":"road.highway","stylers":[{"visibility":"simplified"}]},{"featureType":"road.local","elementType":"labels","stylers":[{"visibility":"off"}]}]';

class MyGoogleMap extends StatefulWidget {
  const MyGoogleMap({Key key}) : super(key: key);

  static GoogleMapController _controller;
  static GoogleMapController get mapController => _controller;
  static set _mapController(GoogleMapController c) => _controller = c;

  @override
  _MyGoogleMapState createState() => _MyGoogleMapState();
}

class _MyGoogleMapState extends State<MyGoogleMap> {
  BitmapDescriptor _markerIcon;
  Future<void> _loadMarkerIcon() async {
    BitmapDescriptor.fromAssetImage(
      ImageConfiguration(),
      'images/location_on.png',
    ).then((value) {
      setState(() {
        _markerIcon = value;
      });
    });
  }

  Set<Marker> _buildMarkers(BuildContext context) {
    var provider = Provider.of<NodeLocationModel>(context);
//    var unlistenedProvider =
//        Provider.of<NodeLocationModel>(context, listen: false);

    Set<Marker> markers = Set<Marker>();

    if (provider.nodeLocations != null) {
      provider.nodeLocations.forEach((element) {
        markers.add(
          Marker(
            markerId: MarkerId(element.id.toString()),
            position: LatLng(element.latitude, element.longitude),
            icon: _markerIcon,
            infoWindow: InfoWindow(
              title: element.name,
              snippet: element.space > 0
                  ? '${element.space} available'
                  : 'Fully occupied',
              onTap: () {
                Navigator.push(
                  context,
                  MaterialPageRoute(
                      builder: (context) =>
                          LocationDetailPage(element.id, element.name)),
                );
              },
            ),
            onTap: () {
              MyGoogleMap.mapController.animateCamera(
                CameraUpdate.newCameraPosition(
                  CameraPosition(
                    target: LatLng(
                      element.latitude,
                      element.longitude,
                    ),
                    zoom: 15.0,
                  ),
                ),
              );
            },
          ),
        );
      });

      return markers;
    }

    return markers;
  }

  @override
  void initState() {
    super.initState();
    _loadMarkerIcon();
  }

  @override
  Widget build(BuildContext context) {
    var provider = Provider.of<NodeLocationModel>(context);
    var unlistenedProvider =
        Provider.of<NodeLocationModel>(context, listen: false);

    if (provider.permissionLocationStatus.isGranted) {
      return Stack(
        children: <Widget>[
          GoogleMap(
            compassEnabled: false,
            mapToolbarEnabled: false,
            rotateGesturesEnabled: false,
            scrollGesturesEnabled: true,
            zoomControlsEnabled: false,
            zoomGesturesEnabled: true,
            tiltGesturesEnabled: false,
            myLocationEnabled: true,
            myLocationButtonEnabled: false,
            buildingsEnabled: false,
            markers: _buildMarkers(context),
            initialCameraPosition: _DEFAULT_CAMERA_POSITION,
            onMapCreated: (controller) async {
              controller.setMapStyle(_DEFAULT_MAP_STYLE);
              MyGoogleMap._mapController = controller;
            },
          ),
          Positioned(
            right: 7.5,
            bottom: 7.5,
            child: FloatingActionButton(
              mini: true,
              backgroundColor: Color(ColorStandard.text),
              child: Icon(Icons.my_location),
              onPressed: () {
                if (unlistenedProvider.position.latitude != null &&
                    unlistenedProvider.position.longitude != null) {
                  MyGoogleMap.mapController.animateCamera(
                    CameraUpdate.newCameraPosition(
                      CameraPosition(
                        target: LatLng(
                          unlistenedProvider.position.latitude,
                          unlistenedProvider.position.longitude,
                        ),
                        zoom: 15.0,
                      ),
                    ),
                  );
                }
              },
            ),
          ),
        ],
      );
    } else {
      return Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: <Widget>[
          Text(
            'Please permit location use',
            style: TextStyle(
              fontFamily: 'Source Sans Pro',
              fontWeight: FontWeight.w700,
              fontSize: 18.0,
              color: Color(ColorStandard.text),
            ),
          ),
          RaisedButton(
            onPressed: provider.requestLocationPermission,
            color: Color(ColorStandard.text),
            textColor: Colors.white,
            child: Text('ALLOW'),
          ),
        ],
      );
    }
  }
}
