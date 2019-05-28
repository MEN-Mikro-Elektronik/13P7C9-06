<?xml version='1.0' encoding='ISO-8859-1' standalone='yes'?>
<tagfile>
  <compound kind="page">
    <filename>index</filename>
    <title></title>
    <name>index</name>
  </compound>
  <compound kind="file">
    <name>pi7c9_gpio_doc.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/PI7C9_GPIO/DRIVER/COM/</path>
    <filename>pi7c9__gpio__doc_8c</filename>
  </compound>
  <compound kind="file">
    <name>pi7c9_gpio_drv.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/PI7C9_GPIO/DRIVER/COM/</path>
    <filename>pi7c9__gpio__drv_8c</filename>
    <class kind="struct">LL_HANDLE</class>
    <member kind="define">
      <type>#define</type>
      <name>_NO_LL_HANDLE</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>CH_NUMBER</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>USE_IRQ</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADDRSPACE_COUNT</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>ADDRSPACE_SIZE</name>
      <anchor>a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBG_MYLEVEL</name>
      <anchor>a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>DBH</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GPIO_REG</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>GPIO_COUNT</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>PI7C9_GPIO_Init</name>
      <anchor>a9</anchor>
      <arglist>(DESC_SPEC *descSpec, OSS_HANDLE *osHdl, MACCESS *ma, OSS_SEM_HANDLE *devSemHdl, OSS_IRQ_HANDLE *irqHdl, LL_HANDLE **llHdlP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>PI7C9_GPIO_Exit</name>
      <anchor>a10</anchor>
      <arglist>(LL_HANDLE **llHdlP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>PI7C9_GPIO_Read</name>
      <anchor>a11</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 *value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>PI7C9_GPIO_Write</name>
      <anchor>a12</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>PI7C9_GPIO_SetStat</name>
      <anchor>a13</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 code, INT32_OR_64 value32_or_64)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>PI7C9_GPIO_GetStat</name>
      <anchor>a14</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, int32 code, INT32_OR_64 *value32_or_64P)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>PI7C9_GPIO_BlockRead</name>
      <anchor>a15</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size, int32 *nbrRdBytesP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>PI7C9_GPIO_BlockWrite</name>
      <anchor>a16</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 ch, void *buf, int32 size, int32 *nbrWrBytesP)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>PI7C9_GPIO_Irq</name>
      <anchor>a17</anchor>
      <arglist>(LL_HANDLE *llHdl)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>PI7C9_GPIO_Info</name>
      <anchor>a18</anchor>
      <arglist>(int32 infoType,...)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>char *</type>
      <name>Ident</name>
      <anchor>a19</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>int32</type>
      <name>Cleanup</name>
      <anchor>a20</anchor>
      <arglist>(LL_HANDLE *llHdl, int32 retCode)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>__PI7C9_GPIO_GetEntry</name>
      <anchor>a21</anchor>
      <arglist>(LL_ENTRY *drvP)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>pi7c9_gpio_drv.h</name>
    <path>/opt/menlinux/INCLUDE/COM/MEN/</path>
    <filename>pi7c9__gpio__drv_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>PI7C9_GPIO_SET_PORTS</name>
      <anchor>a0</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PI7C9_GPIO_CLR_PORTS</name>
      <anchor>a1</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PI7C9_GPIO_DIRECTION</name>
      <anchor>a2</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PI7C9_GPIO_VARIANT</name>
      <anchor>a3</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>_PI7C9_GPIO_GLOBNAME</name>
      <anchor>a4</anchor>
      <arglist>(var, name)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>PI7C9_GPIO_GLOBNAME</name>
      <anchor>a5</anchor>
      <arglist>(var, name)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>__PI7C9_GPIO_GetEntry</name>
      <anchor>a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>INT32_OR_64</name>
      <anchor>a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>U_INT32_OR_64</name>
      <anchor>a8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>INT32_OR_64</type>
      <name>MDIS_PATH</name>
      <anchor>a9</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>pi7c9_gpio_simp.c</name>
    <path>/opt/menlinux/DRIVERS/MDIS_LL/PI7C9_GPIO/EXAMPLE/PI7C9_GPIO_SIMP/COM/</path>
    <filename>pi7c9__gpio__simp_8c</filename>
    <member kind="function" static="yes">
      <type>void</type>
      <name>usage</name>
      <anchor>a0</anchor>
      <arglist>(void)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>void</type>
      <name>PrintError</name>
      <anchor>a1</anchor>
      <arglist>(char *info)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>main</name>
      <anchor>a2</anchor>
      <arglist>(int argc, char *argv[])</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>LL_HANDLE</name>
    <filename>structLL__HANDLE.html</filename>
    <member kind="variable">
      <type>int32</type>
      <name>memAlloc</name>
      <anchor>o0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>OSS_HANDLE *</type>
      <name>osHdl</name>
      <anchor>o1</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DESC_HANDLE *</type>
      <name>descHdl</name>
      <anchor>o2</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MACCESS</type>
      <name>maGpio</name>
      <anchor>o3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>MDIS_IDENT_FUNCT_TBL</type>
      <name>idFuncTbl</name>
      <anchor>o4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>dbgLevel</name>
      <anchor>o5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DBG_HANDLE *</type>
      <name>dbgHdl</name>
      <anchor>o6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>pciBus</name>
      <anchor>o7</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>pciDev</name>
      <anchor>o8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>u_int32</type>
      <name>pciFunc</name>
      <anchor>o9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int32</type>
      <name>DevID</name>
      <anchor>o10</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>pi7c9dummy</name>
    <title>MEN logo</title>
    <filename>pi7c9dummy</filename>
  </compound>
</tagfile>
