#!/usr/bin/env python3
"""Generates the Chitchat OPIs (Phoebus .bob) of the kria IOC:
   opi/chitchat/chitchat.bob       - overview of both links
   opi/chitchat/chitchat_link.bob  - details of one link (macro num = CHCH_CH0/CHCH_CH1)
Usage: gen_chch_opi.py <ioc>/opi"""
import os
import sys
from xml.sax.saxutils import escape

C = {
    "BACKGROUND": (220, 225, 221), "GROUP-BORDER": (150, 155, 151), "GROUP-BACKGROUND": (200, 205, 201),
    "PRIMARY-HEADER-BACKGROUND": (151, 188, 202), "HEADER-TEXT": (0, 0, 0), "MAJOR": (252, 13, 27),
    "ON": (70, 255, 70), "OFF": (60, 100, 60), "WARNING": (252, 242, 17),
}


def color(tag, name, ind="  "):
    r, g, b = C[name]
    return f'{ind}<{tag}>\n{ind}  <color name="{name}" red="{r}" green="{g}" blue="{b}">\n{ind}  </color>\n{ind}</{tag}>\n'


FONT_H1 = '<font>\n  <font name="Header 1" family="Source Sans Pro" style="BOLD_ITALIC" size="36.0">\n  </font>\n</font>\n'
FONT_H2 = '<font>\n  <font name="Header 2" family="Source Sans Pro" style="BOLD_ITALIC" size="21.0">\n  </font>\n</font>\n'
FONT_B = '<font>\n  <font name="Default Bold" family="Source Sans Pro" style="BOLD" size="16.0">\n  </font>\n</font>\n'


class Bob:
    def __init__(self):
        self.w = []
        self.n = 0

    def _name(self, base):
        self.n += 1
        return f"{base}_{self.n}"

    def add(self, s):
        self.w.append(s)

    def rect(self, x, y, w, h, bg="GROUP-BACKGROUND", border="GROUP-BORDER", lw=2):
        self.add(f'<widget type="rectangle" version="2.0.0">\n<name>{self._name("Rect")}</name>\n'
                 f'<x>{x}</x>\n<y>{y}</y>\n<width>{w}</width>\n<height>{h}</height>\n<line_width>{lw}</line_width>\n'
                 + color("line_color", border, "") + color("background_color", bg, "")
                 + '<corner_width>10</corner_width>\n<corner_height>10</corner_height>\n</widget>\n')

    def label(self, x, y, w, h, text, font="", align=0, fg=None, tooltip=None):
        s = (f'<widget type="label" version="2.0.0">\n<name>{self._name("Label")}</name>\n<text>{escape(text)}</text>\n'
             f'<x>{x}</x>\n<y>{y}</y>\n<width>{w}</width>\n<height>{h}</height>\n{font}')
        if fg:
            s += color("foreground_color", fg, "")
        if tooltip:
            s += f"<tooltip>{escape(tooltip)}</tooltip>\n"
        s += f"<horizontal_alignment>{align}</horizontal_alignment>\n<vertical_alignment>1</vertical_alignment>\n</widget>\n"
        self.add(s)

    def update(self, x, y, w, h, pv, fmt=None, align=1):
        s = (f'<widget type="textupdate" version="2.0.0">\n<name>{self._name("Update")}</name>\n<pv_name>{pv}</pv_name>\n'
             f'<x>{x}</x>\n<y>{y}</y>\n<width>{w}</width>\n<height>{h}</height>\n')
        if fmt is not None:
            s += f"<format>{fmt}</format>\n"
        s += f"<horizontal_alignment>{align}</horizontal_alignment>\n<vertical_alignment>1</vertical_alignment>\n</widget>\n"
        self.add(s)

    def entry(self, x, y, w, h, pv):
        self.add(f'<widget type="textentry" version="3.0.0">\n<name>{self._name("Entry")}</name>\n<pv_name>{pv}</pv_name>\n'
                 f'<x>{x}</x>\n<y>{y}</y>\n<width>{w}</width>\n<height>{h}</height>\n'
                 '<horizontal_alignment>1</horizontal_alignment>\n</widget>\n')

    def led(self, x, y, size, pv, bit=-1, on="MAJOR", off="ON", tooltip=None):
        s = (f'<widget type="led" version="2.0.0">\n<name>{self._name("LED")}</name>\n<pv_name>{pv}</pv_name>\n'
             f'<bit>{bit}</bit>\n<x>{x}</x>\n<y>{y}</y>\n<width>{size}</width>\n<height>{size}</height>\n'
             + color("off_color", off, "") + color("on_color", on, ""))
        if tooltip:
            s += f"<tooltip>{escape(tooltip)}</tooltip>\n"
        self.add(s + "</widget>\n")

    def checkbox(self, x, y, w, h, pv, bit, tooltip=None):
        s = (f'<widget type="checkbox" version="2.0.0">\n<name>{self._name("Check")}</name>\n<pv_name>{pv}</pv_name>\n'
             f'<bit>{bit}</bit>\n<label></label>\n<x>{x}</x>\n<y>{y}</y>\n<width>{w}</width>\n<height>{h}</height>\n')
        if tooltip:
            s += f"<tooltip>{escape(tooltip)}</tooltip>\n"
        self.add(s + "</widget>\n")

    def write_button(self, x, y, w, h, text, pv, value, confirm=None, tooltip="$(actions)"):
        # confirm: message of a confirmation dialog shown before writing (None: no dialog)
        conf = ""
        if confirm:
            conf = f"<show_confirm_dialog>true</show_confirm_dialog>\n<confirm_message>{escape(confirm)}</confirm_message>\n"
        self.add(f'<widget type="action_button" version="3.0.0">\n<name>{self._name("Button")}</name>\n<actions>\n'
                 f'<action type="write_pv">\n<description>WritePV</description>\n<pv_name>{pv}</pv_name>\n<value>{value}</value>\n</action>\n'
                 f'</actions>\n<text>{escape(text)}</text>\n<x>{x}</x>\n<y>{y}</y>\n<width>{w}</width>\n<height>{h}</height>\n'
                 f'{FONT_B}<tooltip>{escape(tooltip)}</tooltip>\n{conf}</widget>\n')

    def open_button(self, x, y, w, h, text, file, macros=None):
        m = ""
        if macros:
            m = "<macros>\n" + "".join(f"<{k}>{v}</{k}>\n" for k, v in macros.items()) + "</macros>\n"
        self.add(f'<widget type="action_button" version="3.0.0">\n<name>{self._name("Button")}</name>\n<actions>\n'
                 f'<action type="open_display">\n<file>{file}</file>\n{m}<target>tab</target>\n<description>Open Display</description>\n</action>\n'
                 f'</actions>\n<text>{escape(text)}</text>\n<x>{x}</x>\n<y>{y}</y>\n<width>{w}</width>\n<height>{h}</height>\n'
                 f'{FONT_B}<tooltip>$(actions)</tooltip>\n</widget>\n')

    def group_box(self, x, y, w, h, title):
        self.rect(x, y, w, h)
        self.label(x, y, w, 40, title, FONT_H2, align=1)

    def header(self, width, title, subtitle):
        self.add(f'<widget type="rectangle" version="2.0.0">\n<name>Titlebar</name>\n<width>{width}</width>\n<height>50</height>\n'
                 '<line_width>0</line_width>\n' + color("background_color", "PRIMARY-HEADER-BACKGROUND", "") + "</widget>\n")
        self.label(20, 0, 400, 50, title, FONT_H1, fg="HEADER-TEXT")
        self.label(width - 520, 20, 500, 30, subtitle, FONT_H2, align=2, fg="HEADER-TEXT")

    def write(self, path, name, width, height, macros=None):
        m = ""
        if macros:
            m = "  <macros>\n" + "".join(f"    <{k}>{v}</{k}>\n" for k, v in macros.items()) + "  </macros>\n"
        body = "".join(self.w)
        body = "\n".join("  " + l if l else l for l in body.split("\n"))
        with open(path, "w") as f:
            f.write('<?xml version="1.0" encoding="UTF-8"?>\n<!--Generated by gen_chch_opi.py-->\n<display version="2.0.0">\n'
                    f"  <name>{escape(name)}</name>\n{m}  <width>{width}</width>\n  <height>{height}</height>\n"
                    + color("background_color", "BACKGROUND", "  ") + body.rstrip() + "\n</display>\n")


P = "$(acc):$(sys)_$(group)_"
SIGNALS = [("C1Cav", "Cavity 1 - cavity"), ("C1Rev", "Cavity 1 - reverse"),
           ("C2Cav", "Cavity 2 - cavity"), ("C2Rev", "Cavity 2 - reverse")]
# Interlock inputs of a link in bit order (ChchInterlockBit / chitchat_control.vhd):
# (label, counter name, pass counter name or None, timestamp name) - short PV
# name parts as in kriaApp/Db/chch.substitutions (record names <= 60 chars)
INTERLOCKS = [
    ("Wrong frame", "WrongFrame", None, "WrongFrame"),
    ("CRC fault", "CrcFault", None, "CrcFault"),
    ("Wrong protocol", "WrongProt", None, "WrongProt"),
    ("Timeout", "Timeout", None, "Timeout"),
    ("Error", "Error", None, "Error"),
] + [(f"|IQ|² {t.lower()}", f"{s}Dur", f"{s}Pass", s) for s, t in SIGNALS] + [
    ("RFS interlock cav 1", "RfsC1Ilock", None, "RfsC1Ilock"),
    ("RFS interlock cav 2", "RfsC2Ilock", None, "RfsC2Ilock"),
    ("RFS quench cav 1", "RfsC1Quench", None, "RfsC1Quench"),
    ("RFS quench cav 2", "RfsC2Quench", None, "RfsC2Quench"),
]


def link_panel(path):
    b = Bob()
    W, H = 1640, 1040
    n = P + "$(num):"
    b.header(W, "RFPI Chitchat", "Link $(num)")

    # Frame / link state
    b.group_box(20, 70, 520, 330, "Link")
    rows = [("Location", "Location_r", None), ("Protocol version", "ProtocolVersion_r", None),
            ("Gateware type", "GatewareType_r", None), ("TX frame counter", "FrameCounter_r", None),
            ("Loopback counter", "LoopbackCounter_r", None), ("Round trip latency", "RoundTripLatency_r", None),
            ("Faults / error", "Faults_r", 4), ("RFS status", "Status_r", 4)]
    for i, (t, pv, fmt) in enumerate(rows):
        y = 110 + i * 35
        b.label(40, y, 220, 30, t)
        b.update(270, y, 240, 30, n + pv, fmt)

    # RFS status bits and cavity interlocks
    b.group_box(560, 70, 520, 330, "RFS status / interlocks")
    b.label(740, 110, 150, 30, "Cavity 1", FONT_B, align=1)
    b.label(900, 110, 150, 30, "Cavity 2", FONT_B, align=1)
    st = [("RFS quench", "RfsC{}Quench_r", "MAJOR", "ON"), ("RFS interlock", "RfsC{}Ilock_r", "MAJOR", "ON"),
          ("Interlock output", "C{}Interlock_r", "MAJOR", "ON")]
    for i, (t, pv, on, off) in enumerate(st):
        y = 150 + i * 45
        b.label(580, y, 160, 30, t)
        for k in (1, 2):
            b.led(800 + (k - 1) * 160, y, 30, n + pv.format(k), on=on, off=off)
    y = 150 + 3 * 45
    b.label(580, y, 160, 30, "I/Q select (last)")
    for k in (1, 2):
        b.update(760 + (k - 1) * 160, y, 110, 30, n + f"C{k}IqSelect_r")
    b.write_button(700, 340, 240, 40, "Reset interlocks", n + "InterlockReset_s", 1)

    # Signals
    b.group_box(1100, 70, 520, 330, "Signals (latest I/Q pair)")
    cols = [("Raw", 70), ("I", 70), ("Q", 70), ("|IQ|²", 100)]
    x0 = 1270
    xs = []
    x = x0
    for t, w in cols:
        b.label(x, 110, w, 30, t, FONT_B, align=1)
        xs.append(x)
        x += w + 5
    for i, (s, t) in enumerate(SIGNALS):
        y = 150 + i * 45
        b.label(1115, y, 155, 30, t)
        for (ct, w), xx, suf in zip(cols, xs, ["Raw_r", "I_r", "Q_r", "Amp_r"]):
            b.update(xx, y, w, 30, n + s + suf)

    # Interlock inputs
    b.group_box(20, 420, 1600, 600, "Interlock inputs")
    hdr = [("Input", 190), ("Mask", 50), ("Curr", 50), ("Trig", 50), ("Counter", 90), ("Limit", 90), ("Set limit", 100),
           ("Pass ctr", 90), ("Pass limit", 90), ("Set pass limit", 110), ("|IQ|² limit", 100),
           ("Set |IQ|² limit", 120), ("Overflow timestamp", 230)]
    x = 40
    hx = []
    for t, w in hdr:
        b.label(x, 460, w, 30, t, FONT_B, align=1)
        hx.append((x, w))
        x += w + 10
    for bit, (t, ctr, pas, ts) in enumerate(INTERLOCKS):
        y = 495 + bit * 38
        (xi, wi), (xm, wm), (xc, wc), (xt, wt), (xctr, wctr), (xl, wl), (xs_, ws_), \
            (xp, wp), (xpl, wpl), (xps, wps), (xa, wa), (xas, was), (xts, wts) = hx
        b.label(xi, y, wi, 30, f"{bit:2d}  {t}")
        b.checkbox(xm + 15, y, 30, 30, n + "InterlockMask_s", bit, "Input observed (InterlockMask_s)")
        b.led(xc + 12, y + 2, 26, n + "InterlockCurr_r", bit, tooltip="Current state (InterlockCurr_r)")
        b.led(xt + 12, y + 2, 26, n + "InterlockTrig_r", bit, tooltip="Triggered (InterlockTrig_r)")
        b.update(xctr, y, wctr, 30, n + ctr + "Ctr_r")
        b.update(xl, y, wl, 30, n + ctr + "Limit_r")
        b.entry(xs_, y, ws_, 30, n + ctr + "Limit_s")
        if pas:
            b.update(xp, y, wp, 30, n + pas + "Ctr_r")
            b.update(xpl, y, wpl, 30, n + pas + "Limit_r")
            b.entry(xps, y, wps, 30, n + pas + "Limit_s")
            b.update(xa, y, wa, 30, n + ts + "AmpLimit_r")
            b.entry(xas, y, was, 30, n + ts + "AmpLimit_s")
        b.update(xts, y, wts, 30, n + ts + "Timestamp_r")
    b.write(path, "RFPI Chitchat link", W, H, {"num": "CHCH_CH0"})


def overview_panel(path):
    b = Bob()
    W, H = 1100, 560
    b.header(W, "RFPI Chitchat", "Overview")
    for li in (0, 1):
        x = 20 + li * 540
        num = f"CHCH_CH{li}"
        n = P + num + ":"
        b.group_box(x, 70, 520, 470, f"Link {li} ({num})")
        b.label(x + 200, 110, 140, 30, "Cavity 1", FONT_B, align=1)
        b.label(x + 350, 110, 140, 30, "Cavity 2", FONT_B, align=1)
        for i, (t, pv) in enumerate([("Interlock output", "C{}Interlock_r"), ("RFS interlock", "RfsC{}Ilock_r"),
                                     ("RFS quench", "RfsC{}Quench_r")]):
            y = 150 + i * 45
            b.label(x + 20, y, 180, 30, t)
            for k in (1, 2):
                b.led(x + 255 + (k - 1) * 150, y, 30, n + pv.format(k))
        for i, (t, pv) in enumerate([("Round trip latency", "RoundTripLatency_r"), ("Protocol version", "ProtocolVersion_r"),
                                     ("Triggered inputs", "InterlockTrig_r"), ("Observed inputs (mask)", "InterlockMask_r")]):
            y = 290 + i * 35
            b.label(x + 20, y, 220, 30, t)
            b.update(x + 250, y, 240, 30, n + pv, 4 if "Inter" in pv else None)
        b.write_button(x + 20, 480, 230, 40, "Reset interlocks", n + "InterlockReset_s", 1)
        b.open_button(x + 270, 480, 230, 40, "Details", "chitchat_link.bob", {"num": num})
    b.write(path, "RFPI Chitchat", W, H)


if __name__ == "__main__":
    out = os.path.join(sys.argv[1], "chitchat")
    os.makedirs(out, exist_ok=True)
    overview_panel(os.path.join(out, "chitchat.bob"))
    link_panel(os.path.join(out, "chitchat_link.bob"))
    print("written to", out)
