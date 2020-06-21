import 'dart:convert';
import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:http/http.dart';
import 'package:url_launcher/url_launcher.dart';
import 'package:provider/provider.dart';

import '../common/colors.dart';
import '../models/post_handler.dart';
import '../models/location_model.dart';
import '../models/location_detail_model.dart';
import 'detail_list_tile.dart';

// TODO: Make this page receive live data

class LocationDetailPage extends StatefulWidget {
  LocationDetailPage(this.locationId, this.locationName, {Key key})
      : super(key: key);

  final int locationId;
  final String locationName;

  @override
  _LocationDetailPageState createState() => _LocationDetailPageState();
}

class _LocationDetailPageState extends State<LocationDetailPage> {
  GetHandler _httpHandler = GetHandler(SERVER_URL, SERVER_API_KEY);

  LocationDetail _locationDetail;
  List<LocationDetailFloor> _floorDetails = [];

  Future<void> _loadLocationDetail() async {
    _httpHandler.addParameters('lc_id', widget.locationId.toString());
    Response response = await _httpHandler.getResponse('detail');

    if (response.statusCode == 200) {
      dynamic jsonData = jsonDecode(response.body);

      if (jsonData['status'] == 200) {
        dynamic jsonValues = jsonData['values'];

        setState(() {
          _locationDetail = LocationDetail(
            jsonValues['occupied_space'] as int,
            jsonValues['total_space'] as int,
          );
        });
      }
    }
  }

  Future<void> _loadLocationDetailFloor() async {
    _httpHandler.addParameters('lc_id', widget.locationId.toString());
    Response response = await _httpHandler.getResponse('floor');

    if (response.statusCode == 200) {
      dynamic jsonData = jsonDecode(response.body);

      if (jsonData['status'] == 200) {
        dynamic jsonValues = jsonData['values'];

        setState(() {
          for (int i = 0; i < jsonValues.length; ++i) {
            _floorDetails.add(LocationDetailFloor(
              jsonValues[i]['fl_id'] as int,
              jsonValues[i]['fl_name'],
              jsonValues[i]['floor_occupancy'] as int,
              jsonValues[i]['floor_space'] as int,
            ));
          }
        });
      }
    }
  }

  List<Widget> _buildFloorSnippets() {
    List<Widget> list = [];

    _floorDetails.forEach((element) {
      final double sizedBoxSize = 25.0;

      final freeSpaceTextStyle = TextStyle(
        color: Colors.white,
        fontFamily: 'Source Sans Pro',
        fontWeight: FontWeight.w600,
        fontSize: 12.0,
      );

      Text freeSpace;
      if (element.freeSpace > 99) {
        freeSpace = Text(
          '99+',
          textAlign: TextAlign.center,
          style: freeSpaceTextStyle,
        );
      } else {
        freeSpace = Text(
          element.freeSpace.toString(),
          textAlign: TextAlign.center,
          style: freeSpaceTextStyle,
        );
      }

      list.add(
        Padding(
          padding: EdgeInsets.only(top: 5.0),
          child: Row(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: <Widget>[
              SizedBox(
                height: sizedBoxSize,
                width: sizedBoxSize,
                child: DecoratedBox(
                  decoration: BoxDecoration(
                    color: Color(ColorStandard.text),
                    borderRadius:
                        BorderRadius.all(Radius.circular(sizedBoxSize / 2)),
                  ),
                  child: Center(
                    child: freeSpace,
                  ),
                ),
              ),
              Text(
                ' at',
                style: detailTextStyle.copyWith(
                  fontStyle: FontStyle.italic,
                  fontSize: 20.0,
                ),
              ),
              Text(
                ' floor ${element.name}',
                style: detailTextStyle.copyWith(
                    fontWeight: FontWeight.w600, fontSize: 20.0),
              ),
              Text(
                ' ${element.occupiedSpace}/${element.totalSpace}',
                style: detailTextStyle.copyWith(fontSize: 12.0),
              ),
            ],
          ),
        ),
      );
    });

    return list;
  }

  bool _basicInitializationDone = false;
  Future<void> _initialize() async {
    await _loadLocationDetail();
    _basicInitializationDone = true;

    _loadLocationDetailFloor();
  }

  @override
  void initState() {
    super.initState();
    _initialize();
  }

  @override
  Widget build(BuildContext context) {
//    var provider = Provider.of<NodeLocationModel>(context);
    var unlistenedProvider =
        Provider.of<NodeLocationModel>(context, listen: false);

    int nodeLocationsIndex = unlistenedProvider.nodeLocations
        .indexWhere((element) => element.id == widget.locationId);
    NodeLocation location =
        unlistenedProvider.nodeLocations[nodeLocationsIndex];

    return WillPopScope(
      onWillPop: () async {
        unlistenedProvider.startTimer();
        return true;
      },
      child: Scaffold(
        appBar: AppBar(
          leading: IconButton(
            icon: Icon(Icons.arrow_back),
            onPressed: () {
              Navigator.of(context).pop();
              unlistenedProvider.startTimer();
            },
          ),
          backgroundColor: Color(ColorStandard.text),
          title: Text(
            widget.locationName,
            style: TextStyle(
              fontFamily: 'Source Sans Pro',
            ),
          ),
        ),
        body: _basicInitializationDone
            ? ListView(
                children: <Widget>[
                  DetailListTile(
                    title: 'Overall',
                    snippets: <Widget>[
                      Padding(
                        padding: EdgeInsets.only(top: 5.0),
                        child: Text(
                          '${_locationDetail?.occupiedSpace ?? 0}/${_locationDetail?.totalSpace ?? 0} occupied',
                          style: detailTextStyle.copyWith(fontSize: 20.0),
                        ),
                      ),
                      Padding(
                        padding: EdgeInsets.only(top: 5.0),
                        child: Text(
                          '${_locationDetail?.freeSpace ?? 0} available',
                          style: detailTextStyle.copyWith(fontSize: 20.0),
                        ),
                      ),
                    ],
                  ),
                  _floorDetails.isEmpty
                      ? Container()
                      : DetailListTile(
                          title: 'By floor',
                          snippets: _buildFloorSnippets(),
                        ),
                  SizedBox(height: 20.0),
                  Padding(
                    padding: EdgeInsets.symmetric(horizontal: 20.0),
                    child: Divider(thickness: 1.0),
                  ),
                  Padding(
                    padding: EdgeInsets.only(
                      left: 20.0,
                      right: 20.0,
                      top: 20.0,
                    ),
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                      children: <Widget>[
                        FlatButton(
                          onPressed: () async {
                            final String baseUrl =
                                'https://www.google.com/maps/dir/';

                            String url;
                            url = baseUrl;
                            url += '?api=1';

                            url += '&destination=';
                            url += location.latitude.toString();
                            url += ',';
                            url += location.longitude.toString();

                            url += '&destination_place_id=';
                            url += location.googlePlaceId;

                            url += '&travelmode=driving';

                            if (await canLaunch(url)) {
                              await launch(url);
                            } else {
                              print('[] Could not launch: $baseUrl');
                            }
                          },
                          child: Column(
                            children: <Widget>[
                              Icon(
                                Icons.directions,
                                size: 50.0,
                                color: Color(ColorStandard.text),
                              ),
                              Text(
                                'Directions',
                                style: detailTextStyle.copyWith(
                                  fontSize: 12,
                                  fontWeight: FontWeight.w600,
                                ),
                              )
                            ],
                          ),
                        ),
                        FlatButton(
                          onPressed: () async {
                            final String baseUrl =
                                'https://www.google.com/maps/search/';

                            String url;
                            url = baseUrl;
                            url += '?api=1';

                            url += '&query=';
                            url += location.latitude.toString();
                            url += ',';
                            url += location.longitude.toString();

                            url += '&query_place_id=';
                            url += location.googlePlaceId;

                            if (await canLaunch(url)) {
                              await launch(url);
                            } else {
                              print('[] Could not launch: $baseUrl');
                            }
                          },
                          child: Column(
                            children: <Widget>[
                              Icon(
                                Icons.map,
                                size: 50.0,
                                color: Color(ColorStandard.text),
                              ),
                              Text(
                                'Map',
                                style: detailTextStyle.copyWith(
                                  fontSize: 12,
                                  fontWeight: FontWeight.w600,
                                ),
                              )
                            ],
                          ),
                        ),
                      ],
                    ),
                  ),
                  SizedBox(height: 20.0),
                ],
              )
            : Center(child: CircularProgressIndicator()),
      ),
    );
  }
}
