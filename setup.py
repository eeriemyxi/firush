from setuptools import setup, Extension

firush_api = Extension(
    "firush.api", libraries=["curl"], language="c", sources=["src/py/api/api.c"]
)

setup(
    name="firush",
    version="1.0",
    description="",
    author="myxi",
    author_email="myxi@envs.net",
    packages=["firush"],
    package_dir={"firush": "src/py"},
    entry_points = {"console_scripts": ["firush=firush:main"]},
    install_requires=["nicegui"],
    # package_data={"firush": ["api.pyi", "py.typed"]},
    # url = 'https://docs.python.org/extending/building',
    # long_description = open('README.md').read(),
    ext_modules=[firush_api],
)
