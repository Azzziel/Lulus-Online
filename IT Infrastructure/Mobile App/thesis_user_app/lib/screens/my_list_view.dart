import 'package:flutter/material.dart';
import 'package:google_maps_flutter/google_maps_flutter.dart';
import 'package:provider/provider.dart';

import '../common/colors.dart';
import '../models/location_model.dart';
import 'location_detail_page.dart';
import 'my_google_map.dart';

class MyListView extends StatelessWidget {
  static const listViewTextStyle = TextStyle(
    color: Color(ColorStandard.text),
    fontFamily: 'Source Sans Pro',
  );

  static const divider = Padding(
    padding: EdgeInsets.symmetric(horizontal: 20.0),
    child: Divider(height: 0.0),
  );

  @override
  Widget build(BuildContext context) {
    var provider = Provider.of<NodeLocationModel>(context);
    var unlistenedProvider =
        Provider.of<NodeLocationModel>(context, listen: false);

    // TODO: Handle the situation where there's no internet connection, maybe by adding a retry button
    if (provider.nodeLocations.isEmpty) {
      return Expanded(
        child: Center(child: CircularProgressIndicator()),
      );
    } else {
      return Expanded(
        child: ListView.separated(
          padding: EdgeInsets.zero,
          separatorBuilder: (context, index) => divider,
          itemCount: unlistenedProvider.nodeLocations.length,
          itemBuilder: (context, index) {
            return ListTile(
              trailing: SizedBox(
                height: 45.0,
                width: 45.0,
                child: FlatButton(
                  padding: EdgeInsets.zero,
                  child: Icon(
                    Icons.navigate_next,
                    size: 30.0,
                    color: Color(ColorStandard.text),
                  ),
                  onPressed: () {
                    unlistenedProvider.cancelTimer();
                    Navigator.push(
                      context,
                      MaterialPageRoute(
                        builder: (context) => LocationDetailPage(
                          unlistenedProvider.nodeLocations[index].id,
                          unlistenedProvider.nodeLocations[index].name,
                        ),
                      ),
                    );
                  },
                ),
              ),
              title: Text(
                unlistenedProvider.nodeLocations[index].name,
                style: listViewTextStyle.copyWith(fontWeight: FontWeight.w600),
              ),
              subtitle: MyListSubtitle(index: index),
              leading: MyListTileLead(index: index),
              onTap: () {
                MyGoogleMap.mapController.animateCamera(
                  CameraUpdate.newCameraPosition(
                    CameraPosition(
                      target: LatLng(
                        unlistenedProvider.nodeLocations[index].latitude,
                        unlistenedProvider.nodeLocations[index].longitude,
                      ),
                      zoom: 15,
                    ),
                  ),
                );
              },
            );
          },
        ),
      );
    }
  }
}

class MyListSubtitle extends StatelessWidget {
  const MyListSubtitle({
    Key key,
    @required this.index,
  }) : super(key: key);

  final int index;

  @override
  Widget build(BuildContext context) {
    var provider = Provider.of<NodeLocationModel>(context);
    var unlistenedProvider =
        Provider.of<NodeLocationModel>(context, listen: false);

    String mainSubtitleText;
    if (provider.nodeLocations[index].distance != null) {
      mainSubtitleText =
          (unlistenedProvider.nodeLocations[index].distance / 1000)
              .toStringAsFixed(1);
    } else {
      mainSubtitleText = '-';
    }
    mainSubtitleText += ' km';

    // Defined here not to do unnecessary recalculations
    int closestId = unlistenedProvider.calculateClosestNodeLocationId();

    String trailingText;
    if (unlistenedProvider.nodeLocations[index].id == closestId) {
      trailingText = ' - Nearest';
    } else {
      trailingText = '';
    }

    return Text(
      mainSubtitleText + trailingText,
      style: MyListView.listViewTextStyle.copyWith(fontSize: 10.5),
    );
  }
}

class MyListTileLead extends StatelessWidget {
  const MyListTileLead({
    Key key,
    @required this.index,
  }) : super(key: key);

  final int index;

  @override
  Widget build(BuildContext context) {
//    var provider = Provider.of<NodeLocationModel>(context);
    var unlistenedProvider =
        Provider.of<NodeLocationModel>(context, listen: false);

    final textStyle = TextStyle(
      fontFamily: 'Ubuntu',
      fontWeight: FontWeight.w700,
      color: Colors.white,
    );

    Text myText(String data, TextStyle style) => Text(
          data,
          style: style,
          textAlign: TextAlign.center,
        );

    Text text;
    if (unlistenedProvider.nodeLocations[index].space > 0) {
      if (unlistenedProvider.nodeLocations[index].space > 99) {
        text = myText('99+', textStyle.copyWith(fontSize: 8.0));
      } else {
        text = myText(
          unlistenedProvider.nodeLocations[index].space.toString(),
          textStyle.copyWith(fontSize: 16.0),
        );
      }
    } else {
      text = myText('FULL', textStyle.copyWith(fontSize: 8.0));
    }

    return SizedBox(
      height: 25.0,
      width: 25.0,
      child: DecoratedBox(
        decoration: BoxDecoration(
          color: Color(ColorStandard.text),
          borderRadius: BorderRadius.all(Radius.circular(5.0)),
        ),
        child: Center(
          child: text,
        ),
      ),
    );
  }
}
