FROM alpine:latest

RUN apk --no-cache add \
    g++ \
    cmake \
    git \
    fftw-dev \
    libsndfile-dev \
    make

WORKDIR /app

COPY . /app

RUN make

CMD ["./main"]
