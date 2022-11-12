## SPDX-License-Identifier: MIT
## Copyright (c) 2022 Marco Schmidt <marco.schmidt@taugamma.eu>

BSP = rpi3

TARGET            = aarch64-unknown-none-softfloat
KERNEL_BIN        = kernel8.img
QEMU_BINARY       = qemu-system-aarch64
QEMU_MACHINE_TYPE = raspi3b
#QEMU_RELEASE_ARGS =
QEMU_RELEASE_ARGS = -d in_asm -display none -serial stdio -serial null
OBJDUMP_BINARY    = llvm-objdump
NM_BINARY         = llvm-nm
READELF_BINARY    = llvm-readelf
LD_SCRIPT_PATH    = $(shell pwd)/src/bsp/rpi/
RUSTC_MISC_ARGS   = -C target-cpu=cortex-a53

export LD_SCRIPT_PATH

##--------------------------------------------------------------------------------------------------
## Targets and Prerequisites
##--------------------------------------------------------------------------------------------------
KERNEL_MANIFEST      = Cargo.toml
KERNEL_LINKER_SCRIPT = kernel.ld
LAST_BUILD_CONFIG    = target/$(BSP).build_config

KERNEL_ELF      = target/$(TARGET)/release/kernel
# This parses cargo's dep-info file.
# https://doc.rust-lang.org/cargo/guide/build-cache.html#dep-info-files
KERNEL_ELF_DEPS = $(filter-out %: ,$(file < $(KERNEL_ELF).d)) $(KERNEL_MANIFEST) $(LAST_BUILD_CONFIG)



##--------------------------------------------------------------------------------------------------
## Command building blocks
##--------------------------------------------------------------------------------------------------
RUSTFLAGS = $(RUSTC_MISC_ARGS)                   \
    -C link-arg=--library-path=$(LD_SCRIPT_PATH) \
    -C link-arg=--script=$(KERNEL_LINKER_SCRIPT)

RUSTFLAGS_PEDANTIC = $(RUSTFLAGS) -D warnings -D missing_docs

FEATURES      = --features bsp_$(BSP)
COMPILER_ARGS = --target=$(TARGET) $(FEATURES)  --release

RUSTC_CMD   = cargo rustc $(COMPILER_ARGS) --verbose --bin kernel
DOC_CMD     = cargo doc $(COMPILER_ARGS)
CLIPPY_CMD  = cargo clippy $(COMPILER_ARGS)
OBJCOPY_CMD = rust-objcopy -O binary --strip-all

EXEC_QEMU = $(QEMU_BINARY) -M $(QEMU_MACHINE_TYPE)

.PHONY: all doc qemu clippy clean readelf objdump nm check

all: $(KERNEL_BIN)

$(LAST_BUILD_CONFIG):
	@rm -f target/*.build_config
	@mkdir -p target
	@touch $(LAST_BUILD_CONFIG)

$(KERNEL_ELF): $(KERNEL_ELF_DEPS)
	RUSTFLAGS="$(RUSTFLAGS_PEDANTIC)" $(RUSTC_CMD)

##------------------------------------------------------------------------------
## Generate the stripped kernel binary
##------------------------------------------------------------------------------
$(KERNEL_BIN): $(KERNEL_ELF) objdump
	$(OBJCOPY_CMD) $(KERNEL_ELF) $(KERNEL_BIN)
	@stat -f "%z" $(KERNEL_ELF)
	@stat -f "%z" $(KERNEL_BIN)

doc:
	$(call color_header, "Generating docs")
	$(DOC_CMD) --document-private-items --open


qemu: $(KERNEL_BIN)
	$(call color_header, "Launching QEMU")
	$(EXEC_QEMU) $(QEMU_RELEASE_ARGS) -kernel $(KERNEL_BIN)

clippy:
	@RUSTFLAGS="$(RUSTFLAGS_PEDANTIC)" $(CLIPPY_CMD)

clean:
	rm -rf target $(KERNEL_BIN)

readelf: $(KERNEL_ELF)
	$(READELF_BINARY) --headers $(KERNEL_ELF)

objdump: $(KERNEL_ELF)
	$(OBJDUMP_BINARY) --disassemble --demangle $(KERNEL_ELF) | rustfilt

#--section .text   \
##------------------------------------------------------------------------------
## Run nm
##------------------------------------------------------------------------------
nm: $(KERNEL_ELF)
	$(NM_BINARY) --demangle --print-size $(KERNEL_ELF) | sort | rustfilt
