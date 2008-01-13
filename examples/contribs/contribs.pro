TEMPLATE      = subdirs
SUBDIRS       = agora \
		anaglyph \
		backgroundImage \
		blobWar \
		cornerAxis \
		dvonn \
		quarto \
		terrain \
		textureViewer \
		thumbnail \
		3dsViewer

QT_VERSION = $$[QT_VERSION]
!contains( QT_VERSION, "^4.*" ) {
  SUBDIRS *= eventRecorder x3dViewer
}

