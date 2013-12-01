%define name qemu
%define version 1.6.1

Summary: qemu 
Name: %{name}
Version: %{version}
Release: 1
Vendor: zzl
License: GPL
Group: devel
Source: qemu-1.6.1.tar.bz2
URL: http://www.qemu.org/

%description
qemu is virtual device interface

%prep
%setup -q

%build
./configure --prefix=/usr --libdir=/usr/lib64 --target-list=x86_64-softmmu,i386-softmmu
make

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR="$RPM_BUILD_ROOT" install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
/usr/bin/qemu-ga
/usr/bin/qemu-img
/usr/bin/qemu-io
/usr/bin/qemu-nbd
/usr/bin/qemu-system-i386
/usr/bin/qemu-system-x86_64
/usr/etc/qemu/target-x86_64.conf
/usr/libexec/qemu-bridge-helper
/usr/share/qemu/acpi-dsdt.aml
/usr/share/qemu/bamboo.dtb
/usr/share/qemu/bios.bin
/usr/share/qemu/efi-e1000.rom
/usr/share/qemu/efi-eepro100.rom
/usr/share/qemu/efi-ne2k_pci.rom
/usr/share/qemu/efi-pcnet.rom
/usr/share/qemu/efi-rtl8139.rom
/usr/share/qemu/efi-virtio.rom
/usr/share/qemu/keymaps/ar
/usr/share/qemu/keymaps/bepo
/usr/share/qemu/keymaps/common
/usr/share/qemu/keymaps/da
/usr/share/qemu/keymaps/de
/usr/share/qemu/keymaps/de-ch
/usr/share/qemu/keymaps/en-gb
/usr/share/qemu/keymaps/en-us
/usr/share/qemu/keymaps/es
/usr/share/qemu/keymaps/et
/usr/share/qemu/keymaps/fi
/usr/share/qemu/keymaps/fo
/usr/share/qemu/keymaps/fr
/usr/share/qemu/keymaps/fr-be
/usr/share/qemu/keymaps/fr-ca
/usr/share/qemu/keymaps/fr-ch
/usr/share/qemu/keymaps/hr
/usr/share/qemu/keymaps/hu
/usr/share/qemu/keymaps/is
/usr/share/qemu/keymaps/it
/usr/share/qemu/keymaps/ja
/usr/share/qemu/keymaps/lt
/usr/share/qemu/keymaps/lv
/usr/share/qemu/keymaps/mk
/usr/share/qemu/keymaps/modifiers
/usr/share/qemu/keymaps/nl
/usr/share/qemu/keymaps/nl-be
/usr/share/qemu/keymaps/no
/usr/share/qemu/keymaps/pl
/usr/share/qemu/keymaps/pt
/usr/share/qemu/keymaps/pt-br
/usr/share/qemu/keymaps/ru
/usr/share/qemu/keymaps/sl
/usr/share/qemu/keymaps/sv
/usr/share/qemu/keymaps/th
/usr/share/qemu/keymaps/tr
/usr/share/qemu/kvmvapic.bin
/usr/share/qemu/linuxboot.bin
/usr/share/qemu/multiboot.bin
/usr/share/qemu/openbios-ppc
/usr/share/qemu/openbios-sparc32
/usr/share/qemu/openbios-sparc64
/usr/share/qemu/palcode-clipper
/usr/share/qemu/petalogix-ml605.dtb
/usr/share/qemu/petalogix-s3adsp1800.dtb
/usr/share/qemu/ppc_rom.bin
/usr/share/qemu/pxe-e1000.rom
/usr/share/qemu/pxe-eepro100.rom
/usr/share/qemu/pxe-ne2k_pci.rom
/usr/share/qemu/pxe-pcnet.rom
/usr/share/qemu/pxe-rtl8139.rom
/usr/share/qemu/pxe-virtio.rom
/usr/share/qemu/q35-acpi-dsdt.aml
/usr/share/qemu/qemu-icon.bmp
/usr/share/qemu/qemu_logo_no_text.svg
/usr/share/qemu/s390-ccw.img
/usr/share/qemu/s390-zipl.rom
/usr/share/qemu/sgabios.bin
/usr/share/qemu/slof.bin
/usr/share/qemu/spapr-rtas.bin
/usr/share/qemu/vgabios-cirrus.bin
/usr/share/qemu/vgabios-qxl.bin
/usr/share/qemu/vgabios-stdvga.bin
/usr/share/qemu/vgabios-vmware.bin
/usr/share/qemu/vgabios.bin




