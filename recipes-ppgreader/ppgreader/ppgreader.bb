DESCRIPTION = "A simple ppg reader for virtualppg device"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://main.c \
           file://src/fft.c \
           file://src/threads.c \
           "
S = "${WORKDIR}"

do_compile() {
    set CFLAGS -g
    ${CC} ${CFLAGS} main.c src/fft.c src/threads.c -lpthread -lm ${LDFLAGS} -o ppgreader
    unset CFLAGS
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ppgreader ${D}${bindir}
}