import 'package:http/http.dart';

class GetHandler {
  final String _url;
  final Map<String, String> _headers;

  String _parameters = '';

  GetHandler(this._url, String _apiKey)
      : _headers = {
          'Content-Type': 'application/json',
          'x-api-key': _apiKey,
        };

  void resetParameters() {
    _parameters = '';
  }

  void addParameters(String key, String value) {
    if (_parameters.isEmpty) {
      _parameters += '?';
    } else {
      _parameters += '&';
    }

    _parameters += key;
    _parameters += '=';
    _parameters += value;
  }

  Future<Response> getResponse(String route) async {
    Response response;

    try {
      response = await get(_url + route + _parameters, headers: _headers);
    } catch (e) {
      print('[] Exception details: $e');
    }

    resetParameters();

    return response;
  }
}

/*
class PostHandler {
  PostHandler(this._url, this._authKey) {
    resetAll();
  }

  final String _authKey;
  final String _url;

  Map<String, String> _headers = Map<String, String>();
  Map<String, String> _body = Map<String, String>();

  void addHeaders(Map<String, String> headers) {
    _headers.addAll(headers);
  }

  void addBody(Map<String, String> body) {
    _body.addAll(body);
  }

  void resetAll() {
    resetHeaders();
    resetBody();
  }

  void resetHeaders() {
    _headers.clear();
  }

  void resetBody() {
    _body.clear();
    addBody({'auth_key': _authKey});
  }

  Future<Response> getResponse(String phpName) async {
    Response response;

    try {
      response = await post(_url + phpName, headers: _headers, body: _body);
    } catch (e) {
      print('[] Exception details: $e');
    }

    resetAll();

    return response;
  }
}
*/
