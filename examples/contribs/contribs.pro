TEMPLATE      = subdirs
SUBDIRS       = agora \
		anaglyph \
		backgroundImage \
		blobWar \
		cornerAxis \
		dvonn \
		terrain \
		textureViewer \
		thumbnail

QT_VERSION = $$[QT_VERSION]
!contains( QT_VERSION, "^4.*" ) {
  SUBDIRS *= quarto 3dsViewer eventRecorder
}

