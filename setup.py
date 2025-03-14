from setuptools import Extension, setup

firush_api = Extension(
    "firush.api", libraries=["curl"], language="c", sources=["src/py/api/api.c"]
)

setup(
    name="firush",
    version="1.0",
    description="Find random nHentai manga/doujinshi/comic based on specified search query.",
    author="myxi",
    author_email="myxi@envs.net",
    packages=["firush"],
    package_dir={"firush": "src/py"},
    entry_points={"console_scripts": ["firush=firush:main"]},
    install_requires=["nicegui"],
    url="https://github.com/eeriemyxi/firush/",
    long_description=open("README.md").read(),
    ext_modules=[firush_api],
)
