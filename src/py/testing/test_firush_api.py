import multiprocessing
import os
import pathlib
import http.server

from firush import api

SCRIPT_PATH = pathlib.Path(__file__).parent
PORT = os.environ.get("FIR_TEST_PORT", 9384)
IDS = [
    562039,
    562037,
    562036,
    562031,
    562028,
    562026,
    562025,
    562009,
    562002,
    561994,
    561993,
    561991,
    561987,
    561985,
    561983,
    561982,
    561978,
    561977,
    561972,
    561953,
    561952,
    561951,
    561942,
    561934,
    561915,
]
TOTAL_PAGES = 4825

with open(SCRIPT_PATH / "assets/sample.html") as file:
    HTML = file.read()


class RequestHandler(http.server.SimpleHTTPRequestHandler):
    def do_GET(self):
        self.send_response(200)
        self.send_header("Content-type", "text/html")
        self.end_headers()

        self.wfile.write(HTML.encode())


def start_webserver():
    httpd = http.server.HTTPServer(("localhost", PORT), RequestHandler)
    httpd.serve_forever()


def wait_for_port(host, port):
    import socket

    while True:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            result = s.connect_ex((host, port))
            if result == 0:
                return


def test_fetch_html_for():
    multiprocessing.Process(target=start_webserver, daemon=True).start()
    wait_for_port("localhost", PORT)
    assert api.fetch_html_for(f"http://localhost:{PORT}") == HTML


def test_scrape_page_id():
    assert IDS == api.scrape_page_id(HTML)


def test_scrape_total_pages():
    assert TOTAL_PAGES == api.scrape_total_pages(HTML)
