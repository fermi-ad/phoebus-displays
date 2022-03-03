<?xml version="1.0" encoding="UTF-8"?>
<databrowser>
  <title>Solar Radiation</title>
  <show_toolbar>true</show_toolbar>
  <grid>true</grid>
  <update_period>3.0</update_period>
  <scroll_step>5</scroll_step>
  <scroll>false</scroll>
  <start>2019-05-27 15:30:00.000</start>
  <end>2021-08-18 13:54:07.769</end>
  <archive_rescale>STAGGER</archive_rescale>
  <foreground>
    <red>0</red>
    <green>0</green>
    <blue>0</blue>
  </foreground>
  <background>
    <red>230</red>
    <green>230</green>
    <blue>230</blue>
  </background>
  <title_font>Liberation Sans|22|1</title_font>
  <label_font>Liberation Sans|14|1</label_font>
  <scale_font>Liberation Sans|10|0</scale_font>
  <legend_font>Liberation Sans|12|0</legend_font>
  <axes>
    <axis>
      <visible>true</visible>
      <name>Solar Radiation</name>
      <use_axis_name>true</use_axis_name>
      <use_trace_names>false</use_trace_names>
      <right>false</right>
      <color>
        <red>0</red>
        <green>0</green>
        <blue>0</blue>
      </color>
      <min>-80.0</min>
      <max>875.0</max>
      <grid>true</grid>
      <autoscale>false</autoscale>
      <log_scale>false</log_scale>
    </axis>
  </axes>
  <annotations>
  </annotations>
  <pvlist>
    <pv>
      <display_name>Solar Radiation</display_name>
      <visible>true</visible>
      <name>FNAL-AD-ENV-01:SOLRAD</name>
      <axis>0</axis>
      <color>
        <red>255</red>
        <green>0</green>
        <blue>0</blue>
      </color>
      <trace_type>AREA</trace_type>
      <linewidth>1</linewidth>
      <line_style>SOLID</line_style>
      <point_type>NONE</point_type>
      <point_size>2</point_size>
      <waveform_index>0</waveform_index>
      <period>0.0</period>
      <ring_size>5000</ring_size>
      <request>OPTIMIZED</request>
      <archive>
        <name>archiver2</name>
        <url>pbraw://archiver2.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
    </pv>
  </pvlist>
</databrowser>
