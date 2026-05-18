EXTRA_DIST = Cargo.lock \
	Cargo.toml \
	src/lib.rs

if RUST_DEBUG
CARGO_RELEASE_ARGS=
else
CARGO_RELEASE_ARGS=--release
endif

all-local:
	cd $(top_srcdir)/rust && \
	  CARGO_TARGET_DIR=@top_builddir@/rust/target cargo build $(CARGO_RELEASE_ARGS)

check-local:
	cd $(srcdir)/rust && \
	CARGO_TARGET_DIR=@top_builddir@/rust/target cargo test

clean-local:
	cd $(top_srcdir)/rust && \
	CARGO_TARGET_DIR=@top_builddir@/rust/target cargo clean
