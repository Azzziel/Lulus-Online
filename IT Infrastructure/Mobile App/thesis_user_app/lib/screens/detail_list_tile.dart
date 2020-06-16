import 'package:flutter/material.dart';

import '../common/colors.dart';

const detailTextStyle = TextStyle(
  color: Color(ColorStandard.text),
  fontFamily: 'Source Sans Pro',
);

class DetailListTile extends StatelessWidget {
  const DetailListTile({
    Key key,
    @required this.title,
    this.snippets,
  }) : super(key: key);

  final String title;
  final List<Widget> snippets;

  @override
  Widget build(BuildContext context) {
    return Padding(
      padding: EdgeInsets.only(left: 25.0, right: 25.0, top: 20.0),
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: <Widget>[
          Text(
            title,
            style: detailTextStyle.copyWith(
              fontWeight: FontWeight.w600,
              fontSize: 40.0,
            ),
          ),
          Padding(
            padding: EdgeInsets.only(left: 20.0),
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: snippets,
            ),
          ),
        ],
      ),
    );
  }
}
