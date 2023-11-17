#!/usr/bin/env python3
#coding: utf-8

import os

import licant
import licant.install
from licant.cxx_modules import application
from licant.libs import include

defines = ["NOTRACE=1"]

application("simpleterm",
            sources=[
                "main.cpp"
            ],
            defines=defines,
            optimize="-O0",
            cc_flags="-flto -fPIC -Wextra -Wall -ffunction-sections -fdata-sections",
            cxx_flags="-flto -fPIC -Wextra -Wall -ffunction-sections -fdata-sections",
            ld_flags="-flto -fPIC -static -Wl,--whole-archive -lpthread -Wl,--strip-all -Wl,--no-whole-archive -ffunction-sections -fdata-sections -Wl,--gc-sections",
            libs=["pthread", "readline", "igris", "nos"],
            cxxstd="c++17"
            )


licant.install.install_application(tgt="install_simpleterm", src="simpleterm", dst="simpleterm")

if __name__ == "__main__":
    licant.install.install_application(tgt="install", src="simpleterm", dst="simpleterm")
    licant.ex("simpleterm")
