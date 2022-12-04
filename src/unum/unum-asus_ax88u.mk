# (c) 2022 nearhop.com
# Platform makefile for unum agent executable build

# Add linking flags for libs that we built before the agent
# (those migh differ for the platforms, so include conditionally
# in case we need to share these additions a lot).

CPPFLAGS += -DUSE_OPEN_SSL

# Add hardware kind define
CPPFLAGS += -DDEVICE_PRODUCT_NAME=\"$(MODEL)\"
CPPFLAGS += -DWT_ASSOC_ALT
