
Wymagania.
===============
Aby zbudowa� tranport GG potrzeba:

	- glib >= 2.0.0 (http://www.gtk.org/)
		Przetestowane z glib-2.8.3
	
	- libidn >= 0.3.0 (http://www.gnu.org/software/libidn/)
		Przetestowane z libidn-0.5.17

	- expat >= 1.95.1 (http://expat.sourceforge.net/)
		Przetestowane z expat-1.95.8

	- pkgconfig (http://www.freedesktop.org/software/pkgconfig/)

Kompilacja
==============
Prosta jak w przypadku wi�kszo�ci projekt�w OS:

	./configure
	make

Mo�na u�y� opcji configure "--sysconfdir=", aby ustawi� katalog z plikiem konfiguracyjnym,
lub "--disable-debug", aby wy��czy� kod "odpluskwiaj�cy".

Na FreeBSD i w innych systemach mo�e okaza� si� konieczne takie wywo�anie configure:
	
	./configure CPPFLAGS="-I/usr/local/include" LDFLAGS="-L/usr/local/lib"

inaczej skrypt configure mo�e poprawnie nie wykry� zainstalowanych w systemie
bibliotek, takich jak gettext.

