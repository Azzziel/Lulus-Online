import 'package:flutter/material.dart';

import 'my_google_map.dart';
import 'my_app_bar.dart';
import 'my_list_view.dart';

class HomePage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Container(
        child: Column(
          children: <Widget>[
            Expanded(
              flex: 23,
              child: MyGoogleMap(),
            ),
            Expanded(
              flex: 17,
              child: Container(
                child: Column(
                  children: <Widget>[
                    MyAppBar(),
                    MyListView.divider,
                    MyListView(),
                  ],
                ),
              ),
            ),
          ],
        ),
      ),
    );
  }
}
