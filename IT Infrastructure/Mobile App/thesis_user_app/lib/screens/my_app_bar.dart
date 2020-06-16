import 'package:flutter/material.dart';
import '../common/colors.dart';

class MyAppBar extends StatelessWidget {
  const MyAppBar({Key key}) : super(key: key);

  static const appBarTextStyle = TextStyle(
    fontFamily: 'Ubuntu',
    fontSize: 32.0,
    color: Color(ColorStandard.text),
  );

  @override
  Widget build(BuildContext context) {
    return Container(
      padding: EdgeInsets.symmetric(vertical: 5.0),
      child: Row(
        mainAxisAlignment: MainAxisAlignment.center,
        crossAxisAlignment: CrossAxisAlignment.end,
        children: <Widget>[
          Image.asset(
            'images/brand/icon.png',
            height: 40.0,
          ),
          Text(
            'on',
            style: appBarTextStyle,
          ),
          Text(
            'Spot',
            style: appBarTextStyle.copyWith(fontWeight: FontWeight.w700),
          ),
        ],
      ),
    );
  }
}
