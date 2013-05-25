<include>
  <templates>
    <shapes>
      <shape name="teeter" type="polygon">
        <vertex x="-104" y="-15" />
        <vertex x="104" y="-15" />
        <vertex x="104" y="15" />
        <vertex x="-104" y="15" />
      </shape>
      <shape name="teeter2" type="polygon">
        <vertex x="-15" y="-104" />
        <vertex x="-15" y="104" />
        <vertex x="15" y="104" />
        <vertex x="15" y="-104" />
      </shape>
    </shapes>

    <functions>
      <function name="target_bloat">
        <step>
          <add>
            <time />
            <static value="0.75"/>
          </add>
          <add>
            <static value="1" />
            <mul>
              <sine>
                <mul>
                  <time />
                  <static value="360" />
                </mul>
              </sine>
              <static value="0.25" />
            </mul>
          </add>
          <step>
            <add>
              <time />
              <static value="0.5"/>
            </add>
            <mul>
              <sine>
                <mul>
                  <time />
                  <static value="360" />
                </mul>
              </sine>
              <static value="1.25" />
            </mul>
            <stop />
          </step>
        </step>
      </function>
    </functions>

    <physics>
      <physic name="target">
        <body type="kinematic" angle="0" />
        <fixture density="0.5" friction="0.8" restitution="0.1" />
      </physic>
    </physics>

    <entities>
      <entity name="target_particles" rep="--" collideWithBall="false">
        <animations>
          <animation copies="20" texture="sparkles" blending="add" width="32" height="32" midx="16" midy="16" rotate="true">
            <constants time="0" size="1" />
            <frameindex frames="8">
              <step>
                <setVar name="time">
                  <sub>
                    <time />
                    <static value="0.40" />
                  </sub>
                </setVar>
                <cache>
                  <int>
                    <random min="0" max="8" />
                  </int>
                </cache>
                <stop />
              </step>
            </frameindex>
            <position axis="x">
              <step>
                <add>
                  <var name="time" />
                  <static value="1" />
                </add>
                <static value="0" />
                <mul>
                  <pow>
                    <max>
                      <var name="time" />
                      <static value="0" />
                    </max>
                    <static value="0.5" />
                  </pow>
                  <cache>
                    <random min="-80" max="80" />
                  </cache>
                </mul>
              </step>
            </position>
            <position axis="y">
              <step>
                <add>
                  <var name="time" />
                  <static value="1" />
                </add>
                <static value="0" />
                <mul>
                  <pow>
                    <max>
                      <var name="time" />
                      <static value="0" />
                    </max>
                    <static value="0.5" />
                  </pow>
                  <cache>
                    <random min="-80" max="80" />
                  </cache>
                </mul>
              </step>
            </position>
            <scale axis="x">
              <cache>
                <setVar name="size">
                  <random min="0.3" max="1.0" />
                </setVar>
              </cache>
            </scale>
            <scale axis="y">
              <cache>
                <var name="size" />
              </cache>
            </scale>

            <color channel="red">
              <sub>
                <static value="1" />
                <mul>
                  <cache>
                    <random min="0.0" max="0.9" />
                  </cache>
                  <var name="time" />
                </mul>
              </sub>
            </color>
            <color channel="green">
              <sub>
                <static value="1" />
                <mul>
                  <cache>
                    <random min="0.0" max="0.9" />
                  </cache>
                  <var name="time" />
                </mul>
              </sub>
            </color>
            <color channel="blue">
              <sub>
                <static value="1" />
                <mul>
                  <cache>
                    <random min="0.0" max="0.9" />
                  </cache>
                  <var name="time" />
                </mul>
              </sub>
            </color>
            <color channel="alpha">
              <step>
                <add>
                  <var name="time" />
                  <static value="0.5" />
                </add>
                <static value="1" />
                <max>
                  <static value="0" />
                  <sub>
                    <static value="1" />
                    <pow>
                      <mul>
                        <sub>
                          <var name="time" />
                          <static value="0.5" />
                        </sub>
                        <static value="2" />
                      </mul>
                      <static value="0.7" />
                    </pow>
                  </sub>
                </max>
              </step>
            </color>
          </animation>
        </animations>
      </entity>

      <entity name="target_2" rep="--" collideWithBall="false">
        <animations>
          <animation texture="target" width="67" height="67" midx="33.5" midy="33.5" rotate="true">
            <scale axis="x">
              <function name="target_bloat" />
            </scale>
            <scale axis="y">
              <function name="target_bloat" />
            </scale>
          </animation>
        </animations>
      </entity>

      <entity name="target_1" rep="t1" type="target" collideWithBall="false">
        <physics name="target">
          <shape type="circle" radius="30" />
        </physics>
        <collides>
          <spawnEntity name="target_2">
            <spawnEntity name="target_particles">
              <always />
            </spawnEntity>
          </spawnEntity>
        </collides>
        <animations>
          <animation texture="target" width="67" height="67" midx="33.5" midy="33.5" rotate="true" />
        </animations>
      </entity>

    <entity name="bonustarget_1" rep="bt" type="bonustarget" collideWithBall="false">
      <physics name="target">
        <shape type="circle" radius="30" />
      </physics>
      <collides>
        <spawnEntity name="target_2">
          <spawnEntity name="target_particles">
            <always />
          </spawnEntity>
        </spawnEntity>
      </collides>
      <animations>
        <animation texture="bonustarget" width="67" height="67" midx="33.5" midy="33.5" rotate="true" />
      </animations>
    </entity>
    </entities>
  </templates>
</include>