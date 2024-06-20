FROM ubuntu:jammy

RUN apt-get update && apt-get install -y \
    build-essential \
    libsndfile1-dev \
    libfftw3-dev \
    python3 \
    python3-pip \
    && pip install librosa \
    && pip install matplotlib \
    && pip install 'numpy<2.0.0'

WORKDIR /app

CMD ["bash"]