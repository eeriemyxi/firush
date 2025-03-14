FROM python:3.12-alpine AS base

RUN apk add gcc musl-dev curl-dev

COPY . /server
WORKDIR /server

FROM base AS production
RUN pip install .

CMD firush web --port 80
