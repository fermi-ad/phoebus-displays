<?xml version="1.0" encoding="UTF-8"?>
<databrowser>
  <title>HWR7 Power Levels</title>
  <show_toolbar>true</show_toolbar>
  <grid>true</grid>
  <update_period>3.0</update_period>
  <scroll_step>5</scroll_step>
  <scroll>true</scroll>
  <start>-10 minutes</start>
  <end>now</end>
  <archive_rescale>STAGGER</archive_rescale>
  <foreground>
    <red>0</red>
    <green>0</green>
    <blue>0</blue>
  </foreground>
  <background>
    <red>255</red>
    <green>255</green>
    <blue>255</blue>
  </background>
  <title_font>Liberation Sans|18|1</title_font>
  <label_font>Liberation Sans|12|1</label_font>
  <scale_font>Liberation Sans|12|0</scale_font>
  <legend_font>Liberation Sans|14|0</legend_font>
  <axes>
    <axis>
      <visible>true</visible>
      <name>Forward Power</name>
      <use_axis_name>false</use_axis_name>
      <use_trace_names>true</use_trace_names>
      <right>false</right>
      <color>
        <red>0</red>
        <green>0</green>
        <blue>0</blue>
      </color>
      <min>-0.5</min>
      <max>7.5</max>
      <grid>true</grid>
      <autoscale>false</autoscale>
      <log_scale>false</log_scale>
    </axis>
    <axis>
      <visible>true</visible>
      <name>Reflected Power</name>
      <use_axis_name>false</use_axis_name>
      <use_trace_names>true</use_trace_names>
      <right>true</right>
      <color>
        <red>0</red>
        <green>0</green>
        <blue>0</blue>
      </color>
      <min>-0.1</min>
      <max>1.0</max>
      <grid>false</grid>
      <autoscale>false</autoscale>
      <log_scale>false</log_scale>
    </axis>
  </axes>
  <annotations>
  </annotations>
  <pvlist>
    <pv>
      <display_name>Forward Power</display_name>
      <visible>true</visible>
      <name>PIP2IT-RF-AMP-11:InstFwdPower</name>
      <axis>0</axis>
      <color>
        <red>255</red>
        <green>0</green>
        <blue>0</blue>
      </color>
      <trace_type>AREA</trace_type>
      <linewidth>2</linewidth>
      <line_style>SOLID</line_style>
      <point_type>NONE</point_type>
      <point_size>2</point_size>
      <waveform_index>0</waveform_index>
      <period>0.0</period>
      <ring_size>5000</ring_size>
      <request>OPTIMIZED</request>
    </pv>
    <pv>
      <display_name>Reflected Power</display_name>
      <visible>true</visible>
      <name>PIP2IT-RF-AMP-11:InstRefPower</name>
      <axis>1</axis>
      <color>
        <red>0</red>
        <green>0</green>
        <blue>255</blue>
      </color>
      <trace_type>AREA</trace_type>
      <linewidth>2</linewidth>
      <line_style>SOLID</line_style>
      <point_type>NONE</point_type>
      <point_size>2</point_size>
      <waveform_index>0</waveform_index>
      <period>0.0</period>
      <ring_size>5000</ring_size>
      <request>OPTIMIZED</request>
    </pv>
  </pvlist>
</databrowser>
