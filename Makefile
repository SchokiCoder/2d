# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at https://mozilla.org/MPL/2.0/.

CC = cc
INCLUDE = -I /usr/include/SDL2
CFLAGS = -std=c99 -Wall -Wextra $(INCLUDE)
LIBS = -l SDL2 -l SDL2_image -l SDL2_ttf

#INSTALL_BIN_DIR = /usr/bin
#INSTALL_ASSETS_DIR = /usr/share/${APP_NAME}
#INSTALL_TEXTURES_DIR = ${INSTALL_ASSETS_DIR}/textures
#INSTALL_DESKTOP_DIR = /usr/share/applications
#INSTALL_ICONS_DIR = /usr/share/icons/hicolor

#DEFINES = -D PATH_ASSETS="\"${INSTALL_ASSETS_DIR}/\"" -D PATH_TEXTURES="\"${INSTALL_TEXTURES_DIR}/\""

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

chemarium: $(TARGETS)
	$(CC) $(CFLAGS) -Os -o $@ $^

chemarium_d: $(TARGETS)
	$(CC) $(CFLAGS) -g -o $@ $^

config.o: config.c config.h
	$(CC) $(CFLAGS) -c -o $@ $<

entity.o: entity.c entity.h
	$(CC) $(CFLAGS) -c -o $@ $<

game.o: game.c game.h
	$(CC) $(CFLAGS) -c -o $@ $<

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

path.o: path.c path.h
	$(CC) $(CFLAGS) -c -o $@ $<

world.o: world.c world.h
	$(CC) $(CFLAGS) -c -o $@ $<

gui/menu.o: gui/menu.c gui/menu.h
	$(CC) $(CFLAGS) -c -o $@ $<

gui/label.o: gui/label.c gui/label.h
	$(CC) $(CFLAGS) -c -o $@ $<

gui/button.o: gui/button.c gui/button.h
	$(CC) $(CFLAGS) -c -o $@ $<

gui/entry.o: gui/entry.c gui/entry.h
	$(CC) $(CFLAGS) -c -o $@ $<

gui/sprite.o: gui/sprite.c gui/sprite.h
	$(CC) $(CFLAGS) -c -o $@ $<

engine/world.o: engine/world.c engine/world.h
	$(CC) $(CFLAGS) -c -o $@ $<

engine/physics.o: engine/physics.c engine/physics.h
	$(CC) $(CFLAGS) -c -o $@ $<

engine/log.o: engine/log.c engine/log.h
	$(CC) $(CFLAGS) -c -o $@ $<

engine/sstring.o: engine/sstring.c engine/sstring.h
	$(CC) $(CFLAGS) -c -o $@ $<

engine/dict.o: engine/dict.c engine/dict.h
	$(CC) $(CFLAGS) -c -o $@ $<
