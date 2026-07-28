FROM gcc:latest

WORKDIR /app

COPY . .

RUN chmod +x scripts/*.sh || true
RUN cd scripts && ./bleach_build.sh

CMD ["/bin/bash"]