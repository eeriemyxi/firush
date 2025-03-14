import random
import webbrowser

from firush import api, base


def perform_query(query: str | None = None):
    query = input("Query: ") if not query else query
    print(f"SEARCHING with query {query}")

    html = api.fetch_html_for(base.get_query_url(query, 1))
    total_pages = api.scrape_total_pages(html)

    page_number = random.randint(1, total_pages)

    html = api.fetch_html_for(base.get_query_url(query, page_number))
    ids = api.scrape_page_id(html)

    url = f"https://nhentai.net/g/{random.choice(ids)}/"
    print(f"FOUND {url} at page {page_number} from pool of 1-{total_pages} pages.")

    try:
        input("Press enter to open the link in a web browser, or CTRL+c to close.")
        launched = webbrowser.open_new_tab(url)

        if not launched:
            print("ERROR: Could not launch a web browser.")
            exit(1)

        print("Operation successful.")
        perform_query(query)
    except KeyboardInterrupt:
        exit(0)


def main():
    perform_query()
