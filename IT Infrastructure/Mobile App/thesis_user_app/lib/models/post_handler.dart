import 'package:http/http.dart';

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
