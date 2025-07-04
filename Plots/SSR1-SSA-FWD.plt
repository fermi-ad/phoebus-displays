<?xml version="1.0" encoding="UTF-8"?>
<databrowser>
  <title></title>
  <show_legend>true</show_legend>
  <show_toolbar>true</show_toolbar>
  <update_period>3.0</update_period>
  <scroll_step>5</scroll_step>
  <scroll>false</scroll>
  <start>2021-07-08 12:00:00.000</start>
  <end>2021-07-14 19:03:21.039</end>
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
  <title_font>Liberation Sans|20|1</title_font>
  <label_font>Liberation Sans|14|1</label_font>
  <scale_font>Liberation Sans|12|0</scale_font>
  <legend_font>Liberation Sans|14|0</legend_font>
  <axes>
    <axis>
      <visible>true</visible>
      <name>Forward Power (dBm)</name>
      <use_axis_name>true</use_axis_name>
      <use_trace_names>false</use_trace_names>
      <right>false</right>
      <color>
        <red>0</red>
        <green>0</green>
        <blue>0</blue>
      </color>
      <min>-0.72</min>
      <max>0.59</max>
      <grid>false</grid>
      <autoscale>true</autoscale>
      <log_scale>false</log_scale>
    </axis>
  </axes>
  <annotations>
  </annotations>
  <pvlist>
    <pv>
      <display_name>PSU 1</display_name>
      <visible>true</visible>
      <name>$(dev)$(ssa)-01:RFFwd</name>
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
      <archive>
        <name>archiver2</name>
        <url>pbraw://archiver2.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
      <archive>
        <name>vclx4</name>
        <url>pbraw://vclx4.fnal.gov:17668/retrieval</url>
        <key>2</key>
      </archive>
    </pv>
    <pv>
      <display_name>PSU 2</display_name>
      <visible>true</visible>
      <name>$(dev)$(ssa)-02:RFFwd</name>
      <axis>0</axis>
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
      <archive>
        <name>archiver2</name>
        <url>pbraw://archiver2.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
      <archive>
        <name>vclx4</name>
        <url>pbraw://vclx4.fnal.gov:17668/retrieval</url>
        <key>2</key>
      </archive>
    </pv>
    <pv>
      <display_name>PSU 3</display_name>
      <visible>true</visible>
      <name>$(dev)$(ssa)-03:RFFwd</name>
      <axis>0</axis>
      <color>
        <red>255</red>
        <green>127</green>
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
      <archive>
        <name>archiver2</name>
        <url>pbraw://archiver2.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
      <archive>
        <name>vclx4</name>
        <url>pbraw://vclx4.fnal.gov:17668/retrieval</url>
        <key>2</key>
      </archive>
    </pv>
    <pv>
      <display_name>PSU 4</display_name>
      <visible>true</visible>
      <name>$(dev)$(ssa)-04:RFFwd</name>
      <axis>0</axis>
      <color>
        <red>0</red>
        <green>255</green>
        <blue>127</blue>
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
      <archive>
        <name>archiver2</name>
        <url>pbraw://archiver2.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
      <archive>
        <name>vclx4</name>
        <url>pbraw://vclx4.fnal.gov:17668/retrieval</url>
        <key>2</key>
      </archive>
    </pv>
    <pv>
      <display_name>PSU 5</display_name>
      <visible>true</visible>
      <name>$(dev)$(ssa)-05:RFFwd</name>
      <axis>0</axis>
      <color>
        <red>127</red>
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
      <archive>
        <name>archiver2</name>
        <url>pbraw://archiver2.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
      <archive>
        <name>vclx4</name>
        <url>pbraw://vclx4.fnal.gov:17668/retrieval</url>
        <key>2</key>
      </archive>
    </pv>
    <pv>
      <display_name>PSU 6</display_name>
      <visible>true</visible>
      <name>$(dev)$(ssa)-06:RFFwd</name>
      <axis>0</axis>
      <color>
        <red>255</red>
        <green>255</green>
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
      <archive>
        <name>archiver2</name>
        <url>pbraw://archiver2.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
      <archive>
        <name>vclx4</name>
        <url>pbraw://vclx4.fnal.gov:17668/retrieval</url>
        <key>2</key>
      </archive>
    </pv>
    <pv>
      <display_name>PSU 7</display_name>
      <visible>true</visible>
      <name>$(dev)$(ssa)-07:RFFwd</name>
      <axis>0</axis>
      <color>
        <red>0</red>
        <green>255</green>
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
      <archive>
        <name>archiver2</name>
        <url>pbraw://archiver2.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
      <archive>
        <name>vclx4</name>
        <url>pbraw://vclx4.fnal.gov:17668/retrieval</url>
        <key>2</key>
      </archive>
    </pv>
    <pv>
      <display_name>PSU 8</display_name>
      <visible>true</visible>
      <name>$(dev)$(ssa)-08:RFFwd</name>
      <axis>0</axis>
      <color>
        <red>255</red>
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
      <archive>
        <name>archiver2</name>
        <url>pbraw://archiver2.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
      <archive>
        <name>vclx4</name>
        <url>pbraw://vclx4.fnal.gov:17668/retrieval</url>
        <key>2</key>
      </archive>
    </pv>
  </pvlist>
</databrowser>
