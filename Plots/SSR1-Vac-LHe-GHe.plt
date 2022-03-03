<?xml version="1.0" encoding="UTF-8"?>
<databrowser>
  <title>SSR1 Insulating Vacuum</title>
  <show_legend>true</show_legend>
  <show_toolbar>true</show_toolbar>
  <update_period>3.0</update_period>
  <scroll_step>5</scroll_step>
  <scroll>false</scroll>
  <start>2021-04-22 10:00:00.000</start>
  <end>2021-04-27 16:10:16.805</end>
  <archive_rescale>STAGGER</archive_rescale>
  <foreground>
    <red>0</red>
    <green>0</green>
    <blue>0</blue>
  </foreground>
  <background>
    <red>179</red>
    <green>179</green>
    <blue>179</blue>
  </background>
  <title_font>Liberation Sans|20|1</title_font>
  <label_font>Liberation Sans|14|1</label_font>
  <scale_font>Liberation Sans|12|0</scale_font>
  <legend_font>Liberation Sans|14|0</legend_font>
  <axes>
    <axis>
      <visible>true</visible>
      <name>Pressure (torr)</name>
      <use_axis_name>true</use_axis_name>
      <use_trace_names>false</use_trace_names>
      <right>false</right>
      <color>
        <red>0</red>
        <green>0</green>
        <blue>0</blue>
      </color>
      <min>1.0E-9</min>
      <max>1.0E-7</max>
      <grid>true</grid>
      <autoscale>false</autoscale>
      <log_scale>true</log_scale>
    </axis>
    <axis>
      <visible>true</visible>
      <name>LHe Level (% full)</name>
      <use_axis_name>true</use_axis_name>
      <use_trace_names>false</use_trace_names>
      <right>true</right>
      <color>
        <red>0</red>
        <green>0</green>
        <blue>0</blue>
      </color>
      <min>-253.0</min>
      <max>233.0</max>
      <grid>false</grid>
      <autoscale>false</autoscale>
      <log_scale>false</log_scale>
    </axis>
    <axis>
      <visible>true</visible>
      <name>GHe Pressure (torr)</name>
      <use_axis_name>true</use_axis_name>
      <use_trace_names>false</use_trace_names>
      <right>true</right>
      <color>
        <red>0</red>
        <green>0</green>
        <blue>0</blue>
      </color>
      <min>-102.0</min>
      <max>359.0</max>
      <grid>false</grid>
      <autoscale>false</autoscale>
      <log_scale>false</log_scale>
    </axis>
    <axis>
      <visible>true</visible>
      <name>Cavity Temperature</name>
      <use_axis_name>true</use_axis_name>
      <use_trace_names>false</use_trace_names>
      <right>false</right>
      <color>
        <red>0</red>
        <green>0</green>
        <blue>0</blue>
      </color>
      <min>2.0</min>
      <max>2.1</max>
      <grid>false</grid>
      <autoscale>false</autoscale>
      <log_scale>false</log_scale>
    </axis>
  </axes>
  <annotations>
    <annotation>
      <pv>0</pv>
      <time>2021-04-26 10:15:43.237</time>
      <value>8.976857098933318E-9</value>
      <offset>
        <x>82.0</x>
        <y>-57.0</y>
      </offset>
      <text>{0}
{1}, {2}
{3}
Vacuum Testing begins</text>
    </annotation>
    <annotation>
      <pv>1</pv>
      <time>2021-04-23 18:20:15.000</time>
      <value>60.964500427246094</value>
      <offset>
        <x>10.0</x>
        <y>148.0</y>
      </offset>
      <text>{0}
{1}, {2}
Archiver teething problems</text>
    </annotation>
    <annotation>
      <pv>2</pv>
      <time>2021-04-22 11:50:15.000</time>
      <value>23.019166628519695</value>
      <offset>
        <x>20.0</x>
        <y>20.0</y>
      </offset>
      <text>{0}
{1}, {2}
{3}
Archiver teething issues</text>
    </annotation>
  </annotations>
  <pvlist>
    <pv>
      <display_name>Insulating Vacuum</display_name>
      <visible>true</visible>
      <name>PIP2IT-CRYO-SSR1-01:VacP</name>
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
        <name>pbraw://vclx4.fnal.gov:17668/retrieval</name>
        <url>pbraw://vclx4.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
    </pv>
    <pv>
      <display_name>LHe Level</display_name>
      <visible>true</visible>
      <name>PIP2IT-CRYO-SSR1-01:LHeLvl</name>
      <axis>1</axis>
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
        <name>pbraw://vclx4.fnal.gov:17668/retrieval</name>
        <url>pbraw://vclx4.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
    </pv>
    <pv>
      <display_name>GHe Pressure</display_name>
      <visible>true</visible>
      <name>PIP2IT-CRYO-SSR1-01:GHeP</name>
      <axis>2</axis>
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
        <name>pbraw://vclx4.fnal.gov:17668/retrieval</name>
        <url>pbraw://vclx4.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
    </pv>
    <pv>
      <display_name>Cavity 3 Upper</display_name>
      <visible>true</visible>
      <name>PIP2IT-CRYO-SSR1-13:TempK</name>
      <axis>3</axis>
      <color>
        <red>26</red>
        <green>77</green>
        <blue>26</blue>
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
        <name>pbraw://vclx4.fnal.gov:17668/retrieval</name>
        <url>pbraw://vclx4.fnal.gov:17668/retrieval</url>
        <key>1</key>
      </archive>
    </pv>
  </pvlist>
</databrowser>
