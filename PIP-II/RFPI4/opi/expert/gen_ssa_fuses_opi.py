#!/usr/bin/env python3
"""Generates opi/expert/ssa_fuses.bob: SSA board fuses and SSA power of the 4 cavities.
Usage: gen_ssa_fuses_opi.py <ioc>/opi   (widget helpers from opi/chitchat/gen_chch_opi.py)"""
import importlib.util
import os
import sys

OPI = sys.argv[1]
_spec = importlib.util.spec_from_file_location("gen_chch_opi", os.path.join(OPI, "chitchat", "gen_chch_opi.py"))
g = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(g)

P = "$(acc):$(sys)_$(group)_"
# Fuses of a cavity in channel order (SSA_CH0..5 of its row on the SSA board):
# (label, PV prefix of the record set, PV name of the status, PV name of "tripped")
FUSES = [
    ("Spare", "SSA_C{c}", "SpareFuseStatus_r", "SpareFuseTripped_r"),
    ("SSA Ready", "SSA_C{c}", "FuseStatus_r", "FuseTripped_r"),
    ("SSA Permit", "SSA_PERM_C{c}", "FuseStatus_r", "FuseTripped_r"),
    ("SSA DC", "SSADC_PERM_C{c}", "FuseStatus_r", "FuseTripped_r"),
    ("LLRF", "LLRF_PERM_C{c}", "FuseStatus_r", "FuseTripped_r"),
    ("MPS", "MPS_PERM_C{c}", "FuseStatus_r", "FuseTripped_r"),
]


def main():
    b = g.Bob()
    W, H = 1510, 560
    b.header(W, "RFPI SSA", "Fuses")

    b.group_box(20, 70, W - 40, 470, "SSA fuses and power")
    # columns: cavity | SSA bank on/off | power requested | power on | power tripped |
    #          6 x (fuse OK, tripped)
    x_cav, x_bank, x_req, x_on, x_trip, x_f0, fw = 40, 150, 300, 420, 540, 670, 130
    hy = 120
    b.label(x_cav, hy, 100, 50, "Cavity", g.FONT_B, align=1)
    b.label(x_bank, hy, 140, 50, "SSA bank\npower", g.FONT_B, align=1)
    b.label(x_req, hy, 110, 50, "Power\nrequested", g.FONT_B, align=1)
    b.label(x_on, hy, 110, 50, "Power\non", g.FONT_B, align=1)
    b.label(x_trip, hy, 120, 50, "Power off\nby a fuse", g.FONT_B, align=1)
    for k, (name, _, _, _) in enumerate(FUSES):
        x = x_f0 + k * fw
        b.label(x, hy - 10, fw, 30, f"CH{k} {name}", g.FONT_B, align=1)
        b.label(x, hy + 20, fw // 2, 30, "OK", align=1)
        b.label(x + fw // 2, hy + 20, fw // 2, 30, "Tripped", align=1)

    for c in range(1, 5):
        y = 190 + (c - 1) * 80
        b.label(x_cav, y, 100, 40, f"C{c}", g.FONT_H2, align=1)
        # SSA power of the cavity (all 6 channels of its bank): switching it off
        # also clears "Power off by a fuse" and the captured fuse states, so
        # Off + On re-arms the bank after a blown fuse was replaced
        pwr = f"{P}SSA_PERM_C{c}:Power_s"
        b.write_button(x_bank, y, 65, 40, "On", pwr, 1, tooltip=f"Switch the SSA power of cavity {c} on")
        b.write_button(x_bank + 75, y, 65, 40, "Off", pwr, 0,
                       tooltip=f"Switch the SSA power of cavity {c} off (resets the fuse trip)")
        b.led(x_req + 35, y + 2, 36, f"{P}SSA_PERM_C{c}:Power_r", on="ON", off="OFF",
              tooltip="SSA power requested (Power_s)")
        b.led(x_on + 35, y + 2, 36, f"{P}SSA_C{c}:PowerOn_r", on="ON", off="OFF",
              tooltip="SSA power of the cavity actually on")
        b.led(x_trip + 40, y + 2, 36, f"{P}SSA_C{c}:PowerTripped_r", on="MAJOR", off="IDLE",
              tooltip="SSA power switched off by a blown fuse (cleared by switching the power off)")
        for k, (name, rec, status, tripped) in enumerate(FUSES):
            x = x_f0 + k * fw
            r = rec.format(c=c)
            b.led(x + 15, y + 2, 36, f"{P}{r}:{status}", on="ON", off="MAJOR",
                  tooltip=f"CH{k} {name} fuse: green OK, red blown ({r}:{status})")
            b.led(x + fw // 2 + 15, y + 2, 36, f"{P}{r}:{tripped}", on="MAJOR", off="IDLE",
                  tooltip=f"CH{k} {name} fuse switched the SSA power off ({r}:{tripped})")
    b.label(40, 505, W - 80, 30,
            "A blown fuse of any channel switches the SSA power of its cavity off; "
            "\"Tripped\" marks the fuse that did it. SSA bank Off, then On, resets the trip.")
    b.write(os.path.join(OPI, "expert", "ssa_fuses.bob"), "RFPI SSA fuses", W, H)


if __name__ == "__main__":
    # Colours: green - working, dark green (OFF) - power off, grey (IDLE) - no event,
    # red (MAJOR) - problem
    g.C["IDLE"] = (180, 180, 180)
    main()
    print("written", os.path.join(OPI, "expert", "ssa_fuses.bob"))
