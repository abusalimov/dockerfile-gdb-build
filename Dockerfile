FROM base/devel


RUN pacman --quiet --noconfirm -Sy && pacman --quiet --noconfirm -S \
  mingw-w64

RUN pacman --quiet --noconfirm -Sy && pacman --quiet --noconfirm -S \
  git

RUN mkdir -p /opt \
 && chmod a+w /opt

RUN groupadd -r build \
 && useradd --no-log-init -r -m -g build build \
 && echo "build ALL=(root) NOPASSWD:ALL" > /etc/sudoers.d/build && \
    chmod 0440 /etc/sudoers.d/build

WORKDIR /home/build

COPY . ./

RUN chown -R build:build . \
 && chmod a+x *.sh

USER build
