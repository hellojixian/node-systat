# System control extension

This libs provides more functions to access freebsd's kernel value.
the Kernal value is fetched by sysctl(3) in BSD like system

its also provides interfaces to access system hardware infromations.
such as mainboard temperatures, fans speed, voltage, these information is read from mainboard's Super I/O chip. it only build for IT8728F chip. by referencing the datasheet of the chip, it very possible to add more chip supports.