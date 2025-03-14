import urllib.parse


def get_query_url(query: str, page: int) -> str:
    query = urllib.parse.quote_plus(query)
    url = f"https://nhentai.net/search/?q={query}&page={page}"
    return url
