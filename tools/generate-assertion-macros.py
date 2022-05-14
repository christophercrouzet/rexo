#!/usr/bin/env python
# -*- coding: utf-8 -*-

"""Generate the C code related to the assertion macros."""

import os
import re
from typing import (
    Any,
    Mapping,
    Optional,
    Sequence,
)

_HERE = os.path.dirname(__file__)
_SRC_FILE_PATH = os.path.abspath(
    os.path.join(_HERE, os.pardir, "include", "rexo.h")
)

_REPL_EXPR = re.compile(
    (
        r"^/\* Assertion Macro Helpers[\S\s]+(?=^#endif /\* REXO_REXO_H \*/$)"
    ),
    re.MULTILINE
)

_MAX_ARG_COUNT = 8

_SEVERITIES = (
    {
        "name": "REQUIRE",
        "value": "RX_FATAL",
    },
    {
        "name": "CHECK",
        "value": "RX_NONFATAL",
    },
)

_MACRO_DESCS = (
    {
        "public_params": (
            "CONDITION",
        ),
        "helper_params": (
            "CONDITION",
            "SEVERITY",
        ),
        "fn": "rxp_assess_value",
        "fn_args": (
            "RX_PARAM_CONTEXT",
            "!!(CONDITION)",
            "RXP_TRUE",
            "#CONDITION",
            "__FILE__",
            "__LINE__",
            "SEVERITY",
        ),
    },
    {
        "type": "BOOL",
        "public_params": (
            "CONDITION",
        ),
        "helper_params": (
            "CONDITION",
            "EXPECTED",
            "OP",
            "SEVERITY",
        ),
        "fn": "rxp_bool_assess_value",
        "fn_args": (
            "RX_PARAM_CONTEXT",
            "!!(CONDITION)",
            "EXPECTED",
            "#CONDITION",
            "__FILE__",
            "__LINE__",
            "SEVERITY",
        ),
        "ops": (
            {
                "name": "TRUE",
                "values": {
                    "EXPECTED": "RXP_TRUE",
                }
            },
            {
                "name": "FALSE",
                "values": {
                    "EXPECTED": "RXP_FALSE",
                }
            },
        ),
    },
    {
        "type": "INT",
        "public_params": (
            "X1",
            "X2",
        ),
        "helper_variant": "COMPARISON",
        "helper_params": (
            "X1",
            "X2",
            "OP",
            "SEVERITY",
        ),
        "fn": "rxp_int_assess_comparison",
        "fn_args": (
            "RX_PARAM_CONTEXT",
            "(X1)",
            "(X2)",
            "OP",
            "#X1",
            "#X2",
            "__FILE__",
            "__LINE__",
            "SEVERITY",
        ),
        "ops": (
            {
                "name": "EQUAL",
                "values": {
                    "OP": "RXP_OP_EQUAL",
                }
            },
            {
                "name": "NOT_EQUAL",
                "values": {
                    "OP": "RXP_OP_NOT_EQUAL",
                }
            },
            {
                "name": "GREATER",
                "values": {
                    "OP": "RXP_OP_GREATER",
                }
            },
            {
                "name": "LESSER",
                "values": {
                    "OP": "RXP_OP_LESSER",
                }
            },
            {
                "name": "GREATER_OR_EQUAL",
                "values": {
                    "OP": "RXP_OP_GREATER_OR_EQUAL",
                }
            },
            {
                "name": "LESSER_OR_EQUAL",
                "values": {
                    "OP": "RXP_OP_LESSER_OR_EQUAL",
                }
            },
        ),
    },
    {
        "type": "UINT",
        "public_params": (
            "X1",
            "X2",
        ),
        "helper_variant": "COMPARISON",
        "helper_params": (
            "X1",
            "X2",
            "OP",
            "SEVERITY",
        ),
        "fn": "rxp_uint_assess_comparison",
        "fn_args": (
            "RX_PARAM_CONTEXT",
            "(X1)",
            "(X2)",
            "OP",
            "#X1",
            "#X2",
            "__FILE__",
            "__LINE__",
            "SEVERITY",
        ),
        "ops": (
            {
                "name": "EQUAL",
                "values": {
                    "OP": "RXP_OP_EQUAL",
                }
            },
            {
                "name": "NOT_EQUAL",
                "values": {
                    "OP": "RXP_OP_NOT_EQUAL",
                }
            },
            {
                "name": "GREATER",
                "values": {
                    "OP": "RXP_OP_GREATER",
                }
            },
            {
                "name": "LESSER",
                "values": {
                    "OP": "RXP_OP_LESSER",
                }
            },
            {
                "name": "GREATER_OR_EQUAL",
                "values": {
                    "OP": "RXP_OP_GREATER_OR_EQUAL",
                }
            },
            {
                "name": "LESSER_OR_EQUAL",
                "values": {
                    "OP": "RXP_OP_LESSER_OR_EQUAL",
                }
            },
        ),
    },
    {
        "type": "REAL",
        "public_params": (
            "X1",
            "X2",
        ),
        "helper_variant": "COMPARISON",
        "helper_params": (
            "X1",
            "X2",
            "OP",
            "SEVERITY",
        ),
        "fn": "rxp_real_assess_comparison",
        "fn_args": (
            "RX_PARAM_CONTEXT",
            "(X1)",
            "(X2)",
            "OP",
            "#X1",
            "#X2",
            "__FILE__",
            "__LINE__",
            "SEVERITY",
        ),
        "ops": (
            {
                "name": "EQUAL",
                "values": {
                    "OP": "RXP_OP_EQUAL",
                }
            },
            {
                "name": "NOT_EQUAL",
                "values": {
                    "OP": "RXP_OP_NOT_EQUAL",
                }
            },
            {
                "name": "GREATER",
                "values": {
                    "OP": "RXP_OP_GREATER",
                }
            },
            {
                "name": "LESSER",
                "values": {
                    "OP": "RXP_OP_LESSER",
                }
            },
            {
                "name": "GREATER_OR_EQUAL",
                "values": {
                    "OP": "RXP_OP_GREATER_OR_EQUAL",
                }
            },
            {
                "name": "LESSER_OR_EQUAL",
                "values": {
                    "OP": "RXP_OP_LESSER_OR_EQUAL",
                }
            },
        ),
    },
    {
        "type": "REAL",
        "public_params": (
            "X1",
            "X2",
            "TOL",
        ),
        "helper_variant": "FUZZY_COMPARISON",
        "helper_params": (
            "X1",
            "X2",
            "TOL",
            "OP",
            "SEVERITY",
        ),
        "fn": "rxp_real_assess_fuzzy_comparison",
        "fn_args": (
            "RX_PARAM_CONTEXT",
            "(X1)",
            "(X2)",
            "(TOL)",
            "OP",
            "#X1",
            "#X2",
            "__FILE__",
            "__LINE__",
            "SEVERITY",
        ),
        "ops": (
            {
                "name": "FUZZY_EQUAL",
                "values": {
                    "OP": "RXP_OP_EQUAL",
                }
            },
            {
                "name": "FUZZY_NOT_EQUAL",
                "values": {
                    "OP": "RXP_OP_NOT_EQUAL",
                }
            },
        ),
    },
    {
        "type": "STR",
        "public_params": (
            "S1",
            "S2",
        ),
        "helper_variant": "COMPARISON",
        "helper_params": (
            "S1",
            "S2",
            "STR_CASE",
            "OP",
            "SEVERITY",
        ),
        "fn": "rxp_str_assess_comparison",
        "fn_args": (
            "RX_PARAM_CONTEXT",
            "(S1)",
            "(S2)",
            "STR_CASE",
            "OP",
            "#S1",
            "#S2",
            "__FILE__",
            "__LINE__",
            "SEVERITY",
        ),
        "ops": (
            {
                "name": "EQUAL",
                "values": {
                    "STR_CASE": "RXP_STR_CASE_OBEY",
                    "OP": "RXP_OP_EQUAL",
                }
            },
            {
                "name": "NOT_EQUAL",
                "values": {
                    "STR_CASE": "RXP_STR_CASE_OBEY",
                    "OP": "RXP_OP_NOT_EQUAL",
                }
            },
            {
                "name": "EQUAL_NO_CASE",
                "values": {
                    "STR_CASE": "RXP_STR_CASE_IGNORE",
                    "OP": "RXP_OP_EQUAL",
                }
            },
            {
                "name": "NOT_EQUAL_NO_CASE",
                "values": {
                    "STR_CASE": "RXP_STR_CASE_IGNORE",
                    "OP": "RXP_OP_NOT_EQUAL",
                }
            },
        ),
    },
    {
        "type": "PTR",
        "public_params": (
            "X1",
            "X2",
        ),
        "helper_variant": "COMPARISON",
        "helper_params": (
            "X1",
            "X2",
            "OP",
            "SEVERITY",
        ),
        "fn": "rxp_ptr_assess_comparison",
        "fn_args": (
            "RX_PARAM_CONTEXT",
            "(X1)",
            "(X2)",
            "OP",
            "#X1",
            "#X2",
            "__FILE__",
            "__LINE__",
            "SEVERITY",
        ),
        "ops": (
            {
                "name": "EQUAL",
                "values": {
                    "OP": "RXP_OP_EQUAL",
                }
            },
            {
                "name": "NOT_EQUAL",
                "values": {
                    "OP": "RXP_OP_NOT_EQUAL",
                }
            },
        ),
    },
    {
        "type": "PTR",
        "public_params": (
            "X",
            "ALIGNMENT",
        ),
        "helper_variant": "ALIGNMENT",
        "helper_params": (
            "X",
            "ALIGNMENT",
            "SEVERITY",
        ),
        "fn": "rxp_ptr_assess_alignment",
        "fn_args": (
            "RX_PARAM_CONTEXT",
            "(X)",
            "(ALIGNMENT)",
            "#X",
            "__FILE__",
            "__LINE__",
            "SEVERITY",
        ),
        "ops": (
            {
                "name": "ALIGNED",
            },
        ),
    },
)


def _format_macro(
    lines: Sequence[str],
    indent: int,
):
    lines = list(lines)

    if indent:
        for i, line in enumerate(lines):
            lines[i] = "{padding}{line}".format(
                padding=" " * indent,
                line=line,
            )

    for i, line in enumerate(lines[:-1]):
        lines[i] = "{:79s}\\".format(line)

    return "{}\n".format("\n".join(lines))


def _format_helper_macro_name(
    desc: Mapping[str, Any],
):
    type = (
        "" if "type" not in desc
        else "_{}".format(desc["type"])
    )
    variant = (
        "" if "helper_variant" not in desc
        else "_{}".format(desc["helper_variant"])
    )

    return "RXP{type}_DEFINE{variant}_TEST".format(
        type=type,
        variant=variant,
    )


def _define_section_header(
    title: str,
):
    lines = []
    lines.append("{:67s}O-(''Q)".format("/* {title}".format(title=title)))
    lines.append("   {} */".format("-" * 74))
    return "{}\n".format("\n".join(lines))



def _define_helper_macro(
    desc: Mapping[str, Any],
    variadic: bool = False,
    fmt_specifiers: Optional[int] = None,
    indent: int = 0,
):
    if variadic or fmt_specifiers is None:
        fmt_specifiers_params = ("...",)
        fmt_specifiers_args = ("__VA_ARGS__",)
    else:
        fmt_specifiers_params = ("MSG",) + tuple(
            "_{}".format(i) for i in range(fmt_specifiers)
        )
        fmt_specifiers_args = fmt_specifiers_params


    # Signature

    name = _format_helper_macro_name(desc)

    lines = []
    lines.append("#define {}(".format(name))
    lines.append("    {},".format(", ".join(desc["helper_params"])))
    lines.append("    {}".format(", ".join(fmt_specifiers_params)))
    lines.append(")")


    # Contents

    lines.append("    {}(".format(desc["fn"]))

    for i, arg in enumerate(desc["fn_args"]):
        lines.append("        {},".format(arg))

    lines.append("        {}".format(", ".join(fmt_specifiers_args)))
    lines.append("    )")


    return _format_macro(lines, indent)


def _define_assertion_macro(
    desc: Mapping[str, Any],
    severity: str,
    op: Optional[str] = None,
    variadic: bool = False,
    fmt_specifiers: Optional[int] = None,
    indent: int = 0,
):
    values = {} if op is None else op.get("values", {}).copy()
    values.update({
        "SEVERITY": severity["value"],
    })

    if fmt_specifiers is None:
        fmt_specifiers_params = None
        fmt_specifiers_args = ("NULL",) * (_MAX_ARG_COUNT + 1)
    elif variadic:
        fmt_specifiers_params = ("...",)
        fmt_specifiers_args = ("__VA_ARGS__",)
    else:
        fmt_specifiers_params = ("MSG",) + tuple(
            "_{}".format(i) for i in range(fmt_specifiers)
        )
        fmt_specifiers_args = (
            fmt_specifiers_params
            + ("NULL",) * (_MAX_ARG_COUNT - fmt_specifiers)
        )


    # Signature

    name = (
        "RX{type}{severity}{op}{postfix}".format(
            type="" if "type" not in desc else "_{}".format(desc["type"]),
            severity="_{}".format(severity["name"]),
            op="" if op is None else "_{}".format(op["name"]),
            postfix=(
                "" if fmt_specifiers is None
                else (
                    "_MSG" if fmt_specifiers == 0
                    else "_MSG_{}".format(fmt_specifiers)
                )
            ),
        )
    )

    lines = []
    lines.append("#define {}(".format(name))
    lines.append("    {}".format(", ".join(desc["public_params"])))

    if fmt_specifiers_params is not None:
        lines[-1] += ","
        lines.append("    {}".format(", ".join(fmt_specifiers_params)))

    lines.append(")")


    # Contents

    name = _format_helper_macro_name(desc)
    args = tuple(values.get(x, x) for x in desc["helper_params"])

    lines.append("    {}(".format(name))
    lines.append("        {}".format(", ".join(args)))

    if fmt_specifiers_args is not None:
        lines[-1] += ","
        lines.append("        {}".format(", ".join(fmt_specifiers_args)))

    lines.append("    )")

    return _format_macro(lines, indent)


def _define_macros():
    blocks = []

    blocks.append(_define_section_header("Assertion Macro Helpers"))
    blocks.append("\n")

    blocks.append("#if RXP_HAS_VARIADIC_MACROS\n")

    for desc in _MACRO_DESCS:
        blocks.append(
            _define_helper_macro(
                desc,
                variadic=True,
                fmt_specifiers=None,
                indent=4,
            )
        )
        blocks.append("\n")

    blocks.pop()
    blocks.append("#else /* RXP_HAS_VARIADIC_MACROS */\n")

    for desc in _MACRO_DESCS:
        blocks.append(
            _define_helper_macro(
                desc,
                variadic=False,
                fmt_specifiers=_MAX_ARG_COUNT,
                indent=4,
            )
        )
        blocks.append("\n")

    blocks.pop()
    blocks.append("#endif /* RXP_HAS_VARIADIC_MACROS */\n")
    blocks.append("\n")


    blocks.append(_define_section_header("Main Assertion Macros"))
    blocks.append("\n")

    for desc in _MACRO_DESCS:
        for op in desc.get("ops", (None,)):
            for severity in _SEVERITIES:
                blocks.append(
                    _define_assertion_macro(
                        desc,
                        severity,
                        op,
                    )
                )
                blocks.append("\n")


    blocks.append(_define_section_header("Message Assertion Macros"))
    blocks.append("\n")

    blocks.append("#if RXP_HAS_VARIADIC_MACROS\n")

    for desc in _MACRO_DESCS:
        for op in desc.get("ops", (None,)):
            for severity in _SEVERITIES:
                blocks.append(
                    _define_assertion_macro(
                        desc,
                        severity,
                        op,
                        variadic=True,
                        fmt_specifiers=0,
                        indent=4,
                    )
                )
                blocks.append("\n")

    blocks.pop()
    blocks.append("#else /* RXP_HAS_VARIADIC_MACROS */\n")

    for desc in _MACRO_DESCS:
        for op in desc.get("ops", (None,)):
            for severity in _SEVERITIES:
                blocks.append(
                    _define_assertion_macro(
                        desc,
                        severity,
                        op,
                        variadic=False,
                        fmt_specifiers=0,
                        indent=4,
                    )
                )
                blocks.append("\n")

    blocks.pop()
    blocks.append("#endif /* RXP_HAS_VARIADIC_MACROS */\n")
    blocks.append("\n")


    blocks.append(_define_section_header("Extended Message Assertion Macros"))
    blocks.append("\n")

    for desc in _MACRO_DESCS:
        for i in range(1, _MAX_ARG_COUNT + 1):
            for op in desc.get("ops", (None,)):
                for severity in _SEVERITIES:
                    blocks.append(
                        _define_assertion_macro(
                            desc,
                            severity,
                            op,
                            variadic=False,
                            fmt_specifiers=i,
                        )
                    )
                    blocks.append("\n")

    return "".join(blocks)


def run():
    contents = _define_macros()

    with open(_SRC_FILE_PATH, "r") as f:
        buf = f.read()

    match = _REPL_EXPR.search(buf)
    buf = buf[:match.start()] + contents + buf[match.end():]

    with open(_SRC_FILE_PATH, "w") as f:
        f.write(buf)


if __name__ == "__main__":
    run()
