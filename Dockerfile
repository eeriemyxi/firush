FROM python:3.12-alpine AS base

COPY . /server
WORKDIR /server

RUN apk add gcc musl-dev curl-dev
RUN pip install .

FROM base AS production

CMD firush web
