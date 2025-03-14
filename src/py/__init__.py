import argparse

from firush import api


def main():
    parser = argparse.ArgumentParser(description="Firush CLI")
    subparsers = parser.add_subparsers(dest="command", required=True)

    subparsers.add_parser("tui", help="Run the TUI interface")
    
    web_parser = subparsers.add_parser("web", help="Run the Web interface")
    web_parser.add_argument("--host", default="0.0.0.0", help="Host address to bind the web server")
    web_parser.add_argument("--port", type=int, default=8935, help="Port to bind the web server")

    args = parser.parse_args()

    if args.command == "tui":
        from firush import tui
        tui.main()
    elif args.command == "web":
        from firush import web
        web.main(host=args.host, port=args.port)
