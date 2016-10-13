# Maintainer: enra64 <oerntec.development@gmail.com>
pkgname=pallilux-gui
pkgver=1
pkgrel=1
pkgdesc="An ambilight-like experience for computers."
arch=('i686' 'x86_64')
url="https://github.com/enra64/PalliLux"
license=('WTFPL')
makedepends=('git' 'qt5-base' 'qt5-charts' 'libx11')
depends=('qt5-base' 'qt5-charts' 'libx11')
optdepends=('fftw: for spectrometer functions' 'libpulse: for spectrometer functions')


_gitroot="https://github.com/enra64/PalliLux.git"
_gitname="PalliLux"

build() {
  cd "$srcdir"
  msg "Connecting to GIT server...."

  if [ -d $_gitname ] ; then
    cd $_gitname && git pull origin
    msg "The local files are updated."
  else
    git clone $_gitroot $_gitname
  fi

  msg "GIT checkout done or server timeout"
  msg "Starting make..."

  rm -rf "$srcdir/$_gitname-build"
  git clone "$srcdir/$_gitname" "$srcdir/$_gitname-build"
  cd "$srcdir/$_gitname-build"

  qmake
  make
  make check
  #install -Dm755 pallilux-gui ${pkgdir}/usr/bin/pallilux-gui
}
