FROM gcc:13-bookworm

WORKDIR /app
COPY . .

RUN chmod +x scripts/*.sh \
    && cd scripts \
    && ./bleach_build.sh

WORKDIR /app/scripts/

CMD ["/bin/bash"]