"""Compatibility shim for deprecated :mod:`imp` module."""

from types import ModuleType
import sys


def new_module(name):
    """Return a new module object with the supplied *name*."""
    return ModuleType(name)


def get_tag():
    """Return the import cache tag for the running interpreter."""
    tag = getattr(sys.implementation, "cache_tag", None)
    if tag is None:
        raise AttributeError("cache_tag is not available")
    return tag
