#! /usr/bin/env python3

# This script regenerates TrustInSoft CI configuration.

# Run from the root of the project:
# $ python3 .trustinsoft/regenerate.py

import itertools
import tis

import json
from os import path # path.basename, path.isdir, path.join
from itertools import product  # Cartesian product of lists.

def get_tests_description():
    with open("all_tests.json", "r") as file:
        return json.load(file)

def make_test(test, test_number):
    # UB or not UB
    test_name_errors = "(expected: OK)"
    if test["errors"]:
        test_name_errors = "(expected: UB)"
    # Full name
    test_name = "%s %03d %s" % (test["name"], test_number, test_name_errors)
    # Prefix path
    prefix_path = path.join("..", "02.wo_Defects")
    if test["errors"]:
        prefix_path = path.join("..", "01.w_Defects")
    # Put it all together
    tis_test = {
        "name": test_name,
        "include": "common_config.json",
        "prefix_path": prefix_path,
        "files":
        [
            "main.c",
            "%s.c" % test["name"]
        ],
        "cpp-extra-args": "-D volatile= ",
        "val-args": " %d%03d" % (test["file_number"], test_number)
    }
    # Add the additional file for the "invalid_extern" cases
    if test["name"] == "invalid_extern":
        list.append(tis_test["files"], "invalid_extern_1.c")
    # Cases needing exhaustive rand() implementation and the analyzer mode
    if ((test["name"] == "zero_division" and test_number == 10) or
        (test["name"] == "conflicting_cond") or
        (test["name"] == "redundant_cond") or
        (test["name"] == "data_lost" and test_number == 12) or
        (test["name"] == "double_free" and test_number == 4) or
        (test["name"] == "free_null_pointer" and test_number == 11) or
        (test["name"] == "memory_allocation_failure" and test_number in [1,5,9,10]) or
        (test["name"] == "memory_leak" and test_number in [1,7]) or
        (test["name"] == "uninit_memory_access" and test_number in [14]) or
        (test["name"] == "not_return") or
        False):
        list.append(tis_test["files"], path.join("..", ".trustinsoft", "rand.c"))
        tis_test["val-profile"] = "analyzer"
        tis_test["slevel"] = 1000
    # Cases needing free() implementation that warns when null pointer passed as argument
    if test["name"] == "free_null_pointer":
        list.append(tis_test["files"], path.join("..", ".trustinsoft", "free.c"))
        tis_test["val-builtin"] = { "free": None }
    # Add / remove options for specific cases
    # Cases where pointer arithmetic UB is detected instead of the expected UB
    if test["name"] in ["buffer_overrun_dynamic", "buffer_underrun_dynamic", "null_pointer", "overrun_st", "st_underrun", "underrun_st", "memory_allocation_failure"]:
        tis_test["val-warn-pointer-arithmetic-out-of-bounds"] = False
    # Cases that need the analyzer mode
    if ((test["name"] == "improper_termination_of_block" and test_number == 4) or
        (test["name"] == "endless_loop")):
        tis_test["val-profile"] = "analyzer"
        tis_test["slevel"] = 1000
    # Cases where signed/unsigned downcast/overflow is detected instead of the expected UB
    if test["name"] == "memory_allocation_failure":
        tis_test["warn-signed-downcast"] = False
    #     tis_test["warn-signed-overflow"] = False
        tis_test["warn-unsigned-downcast"] = False
        tis_test["warn-unsigned-overflow"] = False
    # Cases where we are looking for copying/assigning uninitialized values (not officially UB)
    if test["name"] in ["invalid_memory_access", "uninit_memory_access", "uninit_pointer", "uninit_var", "return_local"]:
        tis_test["val-warn-copy-indeterminate"] = "@all"
    # Cases where unspecified access UB is detected instead of the expected UB
    if test["name"] == "uninit_pointer" and test_number == 12:
        tis_test["unspecified-access"] = False
    # Cases where we need to ignore the possible malloc failure in order to find the expected UB
    if ((test["name"] == "double_free" and test_number == 4) or
        (test["name"] == "free_null_pointer" and test_number == 11) or
        (test["name"] == "memory_allocation_failure") or
        (test["name"] == "uninit_memory_access")):
        tis_test["val-malloc-returns-null"] = False
    # Cases which need more precision
    if test["name"] == "memory_leak" and test_number == 7:
        tis_test["val-split-return"] = "full"
    # Done!
    return tis_test

def make_tests(test):
    test_numbers = range(1, test["last_test"] + 1)
    return list(map(
        lambda test_number: make_test(test, test_number),
        test_numbers
    ))

def make_tis_config():
    tests_descriptions = get_tests_description()
    tests = list(itertools.chain(*(
        list(map(
            lambda test: make_tests(test), tests_descriptions
        ))
    )))
    # print(json.dumps(tests, indent=3))
    return tests

tis_config = make_tis_config()
with open("config.json", "w") as file:
    print("Generate the 'tis.config' file.")
    file.write(tis.string_of_json(tis_config))
