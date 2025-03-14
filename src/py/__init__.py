import argparse

from firush import api


def main():
    parser = argparse.ArgumentParser(description="Firush CLI")
    subparsers = parser.add_subparsers(dest="command", required=True)

    subparsers.add_parser("tui", help="Run the TUI interface")
    subparsers.add_parser("web", help="Run the Web interface")

    args = parser.parse_args()

    if args.command == "tui":
        from firush import tui

        tui.main()
    elif args.command == "web":
        from firush import web

        web.main()
