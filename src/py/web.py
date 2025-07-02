import os
import random

from nicegui import app, html, run, ui

from firush import api, base

GRUVBOX_COLORS = {
    "primary": "#427b58",
    "secondary": "#3c3836",
    "accent": "#d3869b",
    "dark": "#282828",
    "dark_page": "#282828",
    "positive": "#b5bd68",
    "negative": "#fb4934",
    "info": "#668cc0",
    "warning": "#f2c037",
}

STORAGE_SECRET = os.environ.get("STORAGE_SECRET", "NOSECRET")


def ts_notify(label, *args):
    with label:
        ui.notify(*args)


def find_manga(query: str, label):
    ts_notify(label, f'Finding a manga for query="{query}"')

    ts_notify(label, "Preparing to scrape total pages...")
    html = api.fetch_html_for(base.get_query_url(query, 1))
    total_pages = api.scrape_total_pages(html)

    page_number = random.randint(1, total_pages)

    ts_notify(label, "Preparing to scrape manga ids...")
    html = api.fetch_html_for(base.get_query_url(query, page_number))
    ids = api.scrape_page_id(html)

    url = f"https://nhentai.net/g/{random.choice(ids)}/"
    ts_notify(
        label, f"Found {url} at page {page_number} from pool of 1-{total_pages} pages."
    )

    with label:
        ui.navigate.to(url, new_tab=True)


async def on_submit(query_inp, useless_label):
    query = query_inp.value

    if app.storage.user["query"] != query:
        app.storage.user["query"] = query
        ui.notify("Successfully saved the state of the query input.")

    await run.io_bound(find_manga, query, useless_label)


@ui.page("/")
def index():
    ui.dark_mode().enable()
    ui.page_title("Firush Web Client")
    ui.colors(**GRUVBOX_COLORS)

    useless_label = ui.label()

    with ui.column(align_items="center", wrap=True).classes("fixed-center"):
        ui.markdown("#**Firush Web Client**").classes("text-positive")
        ui.markdown(
            "Find random [nHentai](https://nhentai.net/) manga/doujinshi/comic based on specified search query."
        ).style("padding-bottom: 20px")

        with ui.row(align_items="center").classes("justify-center"):
            query = ui.input(placeholder="Query").props("rounded outlined dense")
            query.set_value(app.storage.user.get("query"))

        ui.button(
            "Search",
            on_click=lambda: on_submit(query, useless_label),
        ).props(
            "rounded outlined dense"
        ).style("margin-top: 40px; padding: 15px")


def main(host: str, port: int):
    print("[INFO] Starting the web client...")

    ui.run(host=host, port=port, reload=False, show=False, storage_secret=STORAGE_SECRET)


if __name__ in ("__main__", "__mp_main__"):
    main()
