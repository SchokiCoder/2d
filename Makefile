# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

APP_NAME=chemarium
APP_VERSION=0.0.0
APP_LICENSE=MPLv2
APP_LICENSE_SOURCE1=If a copy of the MPL was not distributed with this file,
APP_LICENSE_SOURCE2=You can obtain one at https://mozilla.org/MPL/2.0/.
APP_SOURCE=https://github.com/SchokiCoder/chemarium

BIN_DIR=$(HOME)/.local/bin
TEXTURES_DIR=$(HOME)/.local/share/$(APP_NAME)/textures
#DESKTOP_DIR=/usr/share/applications
#ICONS_DIR=/usr/share/icons/hicolor

DEFINES=-D PATH_TEXTURES="\"$(TEXTURES_DIR)/\"" \
	-D APP_NAME="\"$(APP_NAME)\"" \
	-D APP_VERSION="\"$(APP_VERSION)\"" \
	-D APP_LICENSE="\"$(APP_LICENSE)\"" \
	-D APP_LICENSE_SOURCE1="\"$(APP_LICENSE_SOURCE1)\"" \
	-D APP_LICENSE_SOURCE2="\"$(APP_LICENSE_SOURCE2)\"" \
	-D APP_SOURCE="\"$(APP_SOURCE)\""

CC=cc
INCLUDE=-I . -I /usr/include/SDL2
LIBS=-l SDL2 -l SDL2_image -l SDL2_ttf
CFLAGS=-std=c99 -Wall -Wextra $(INCLUDE) $(LIBS) $(DEFINES)
REL_CFLAGS=$(CFLAGS) -Os
DEB_CFLAGS=$(CFLAGS) -g -D _DEBUG

TARGETS_GUI=gui/sprite.o \
	gui/entry.o \
	gui/button.o \
	gui/label.o \
	gui/menu.o

TARGETS_ENGINE=engine/dict.o \
	engine/sstring.o \
	engine/log.o \
	engine/physics.o \
	engine/world.o

TARGETS=$(TARGETS_ENGINE) $(TARGETS_GUI) \
	world.o path.o main.o game.o entity.o config.o

# the difference between this and chemarium_d sucks but it works for now
chemarium: *.c engine/*.c gui/*.c
	$(CC) $(REL_CFLAGS) -o $@ $^

# this is built more often, so it gets the speed improvement
chemarium_d: $(TARGETS)
	$(CC) $(DEB_CFLAGS) -o $@ $^

config.o: config.c config.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

entity.o: entity.c entity.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

game.o: game.c game.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

main.o: main.c
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

path.o: path.c path.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

world.o: world.c world.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

gui/menu.o: gui/menu.c gui/menu.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

gui/label.o: gui/label.c gui/label.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

gui/button.o: gui/button.c gui/button.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

gui/entry.o: gui/entry.c gui/entry.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

gui/sprite.o: gui/sprite.c gui/sprite.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

engine/world.o: engine/world.c engine/world.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

engine/physics.o: engine/physics.c engine/physics.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

engine/log.o: engine/log.c engine/log.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

engine/sstring.o: engine/sstring.c engine/sstring.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

engine/dict.o: engine/dict.c engine/dict.h
	$(CC) $(DEB_CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGETS)
	rm -f chemarium
	rm -f chemarium_d
