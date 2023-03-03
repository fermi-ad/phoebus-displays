<?xml version="1.0" encoding="UTF-8"?>
<databrowser>
  <title>pHB650 Pump down</title>
  <show_legend>true</show_legend>
  <show_toolbar>true</show_toolbar>
  <grid>true</grid>
  <update_period>3.0</update_period>
  <scroll_step>1</scroll_step>
  <scroll>false</scroll>
  <start>2023-01-13 00:00:00.000</start>
  <end>2023-02-17 12:17:56.083</end>
  <archive_rescale>STAGGER</archive_rescale>
  <foreground>
    <red>0</red>
    <green>0</green>
    <blue>0</blue>
  </foreground>
  <background>
    <red>204</red>
    <green>204</green>
    <blue>204</blue>
  </background>
  <title_font>Liberation Sans|24|1</title_font>
  <label_font>Liberation Sans|14|1</label_font>
  <scale_font>Liberation Sans|12|0</scale_font>
  <legend_font>Liberation Sans|14|0</legend_font>
  <axes>
    <axis>
      <visible>true</visible>
      <name>Vacuum Pressure (mbar)</name>
      <use_axis_name>true</use_axis_name>
      <use_trace_names>false</use_trace_names>
      <right>false</right>
      <color>
        <red>0</red>
        <green>0</green>
        <blue>0</blue>
      </color>
      <min>1.0E-6</min>
      <max>10000.0</max>
      <grid>true</grid>
      <autoscale>false</autoscale>
      <log_scale>true</log_scale>
    </axis>
  </axes>
  <annotations>
  </annotations>
  <pvlist>
    <pv>
      <display_name>Insulating Vacuum - cold cathode gauge</display_name>
      <visible>true</visible>
      <name>PIP2IT:pHB650_VAC_PCC051:VacP</name>
      <axis>0</axis>
      <color>
        <red>255</red>
        <green>0</green>
        <blue>255</blue>
      </color>
      <trace_type>AREA</trace_type>
      <linewidth>1</linewidth>
      <line_style>SOLID</line_style>
      <point_type>NONE</point_type>
      <point_size>1</point_size>
      <waveform_index>0</waveform_index>
      <period>0.0</period>
      <ring_size>5000</ring_size>
      <request>OPTIMIZED</request>
      <archive>
        <name>archiver1</name>
        <url>pbraw://archiver1.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
    </pv>
    <pv>
      <display_name>Insulating Vacuum - Perani gauge)</display_name>
      <visible>true</visible>
      <name>PIP2IT:pHB650_VAC_PPG051:VacP</name>
      <axis>0</axis>
      <color>
        <red>0</red>
        <green>0</green>
        <blue>255</blue>
      </color>
      <trace_type>AREA</trace_type>
      <linewidth>1</linewidth>
      <line_style>SOLID</line_style>
      <point_type>NONE</point_type>
      <point_size>1</point_size>
      <waveform_index>0</waveform_index>
      <period>0.0</period>
      <ring_size>5000</ring_size>
      <request>OPTIMIZED</request>
      <archive>
        <name>archiver1</name>
        <url>pbraw://archiver1.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
    </pv>
  </pvlist>
</databrowser>
