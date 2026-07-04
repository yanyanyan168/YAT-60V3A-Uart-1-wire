# Skill：Windows/HBuilderX/嵌入式项目中文编码安全处理

适用对象：Codex / AI Agent  
适用场景：Windows 项目、HBuilderX、uni-app、嵌入式 C 工程、中文注释、中文日志、中文 Markdown 文档  
目标：避免中文乱码、避免误改编码、避免把可编译工程改坏。

---

## 1. 核心原则

在本项目中，**所有中文内容必须优先保护编码**。

绝对不要在未确认编码的情况下直接读写文件。

尤其不要做这些危险操作：

```text
1. 不要用默认编码直接读取中文文件；
2. 不要用默认编码直接覆盖中文文件；
3. 不要把所有文件盲目转 UTF-8；
4. 不要把旧嵌入式 C 工程文件批量改编码；
5. 不要用乱码内容再次写回文件；
6. 不要为了修一个功能导致中文注释、中文日志全部乱码。
```

---

## 2. 项目默认编码约定

### 2.1 新增文档

新增 Markdown / JSON / JS / Vue 文件，统一使用：

```text
UTF-8 无 BOM
CRLF 换行
```

例如：

```text
*.md
*.json
*.js
*.vue
README_*.md
给CODEX说的话.md
```

### 2.2 微信小程序 / uni-app 工程

小程序新工程默认：

```text
UTF-8 无 BOM
```

涉及中文页面、中文注释、中文提示，必须保持 UTF-8。

### 2.3 MCU 旧工程

MCU 旧工程可能混有：

```text
GBK
GB2312
UTF-8
UTF-8 with BOM
ASCII
```

所以对 MCU 旧源码：

```text
先识别编码，再读取；
能不改编码就不改编码；
只修改需要修改的文件；
写回时尽量保持原编码。
```

---

## 3. 读取中文文件前必须先做编码检测

在 Windows 项目中，读取中文文件前，先用 Python 做编码探测。

推荐脚本：

```python
from pathlib import Path

def detect_encoding(path: str) -> str:
    data = Path(path).read_bytes()

    # BOM 检测
    if data.startswith(b'\xef\xbb\xbf'):
        return 'utf-8-sig'
    if data.startswith(b'\xff\xfe') or data.startswith(b'\xfe\xff'):
        return 'utf-16'

    # 依次尝试常见中文编码
    for enc in ['utf-8', 'gb18030', 'gbk', 'gb2312', 'big5']:
        try:
            data.decode(enc)
            return enc
        except UnicodeDecodeError:
            pass

    # 最后兜底，不能保证一定正确
    return 'gb18030'
```

读取文件时：

```python
from pathlib import Path

path = Path(r'目标文件路径')
enc = detect_encoding(str(path))
text = path.read_text(encoding=enc, errors='strict')
print('encoding =', enc)
```

---

## 4. 写回文件时必须保持原编码

修改旧文件时，必须记录原编码：

```python
path = Path(r'目标文件路径')
enc = detect_encoding(str(path))
old = path.read_text(encoding=enc, errors='strict')

new = old.replace('旧内容', '新内容')

# 写回时保持原编码，不要默认 utf-8
path.write_text(new, encoding=enc, newline='')
```

如果只是新增文件，才使用：

```python
path.write_text(text, encoding='utf-8', newline='\r\n')
```

---

## 5. 禁止用乱码内容作为修改依据

如果读出来出现这种内容：

```text
绋嬪簭璇存槑
鍙傛暟閰嶇疆
涓枃娉ㄩ噴
```

说明已经乱码。

此时必须停止修改，并重新按正确编码读取。

不要把乱码内容再写回文件。

正确流程：

```text
1. 停止修改；
2. 关闭当前乱码视图；
3. 用编码检测脚本判断；
4. 用正确 encoding 重新读取；
5. 确认中文显示正常；
6. 再继续修改。
```

---

## 6. 对 Markdown 文档的处理规则

本项目新生成的 `.md` 文档统一：

```text
UTF-8 无 BOM
CRLF
```

读取 Markdown 时优先：

```text
utf-8-sig
utf-8
gb18030
```

不要直接用系统默认编码。

Python 示例：

```python
def read_text_safely(path):
    data = Path(path).read_bytes()
    for enc in ['utf-8-sig', 'utf-8', 'gb18030', 'gbk']:
        try:
            return data.decode(enc), enc
        except UnicodeDecodeError:
            continue
    raise UnicodeDecodeError('unknown', data, 0, 1, '无法识别编码')
```

---

## 7. 对 HBuilderX / uni-app 文件的处理规则

以下文件必须保持 UTF-8：

```text
pages/**/*.vue
components/**/*.vue
common/**/*.js
store/**/*.js
pages.json
manifest.json
package.json
README_*.md
```

写入时：

```python
Path(file).write_text(text, encoding='utf-8', newline='\r\n')
```

不要写成 GBK。

如果 HBuilderX 显示乱码：

```text
优先检查文件编码；
不要盲目复制乱码内容；
不要重新保存成 ANSI。
```

---

## 8. 对嵌入式 C 文件的处理规则

以下文件可能来自旧 SDK 或旧工程，编码可能不是 UTF-8：

```text
*.c
*.h
*.S
*.s
*.cbp
*.xml
*.txt
```

处理规则：

```text
1. 修改前先备份；
2. 检测编码；
3. 读取确认中文正常；
4. 只做最小修改；
5. 写回保持原编码；
6. 不批量格式化；
7. 不批量转码；
8. 不批量替换换行符；
9. 不动 include_lib，除非明确要求。
```

特别注意：

```text
旧 MCU 工程 include_lib 版本敏感，不要随意替换或转码。
```

---

## 9. 生成给用户的文件时的规则

如果生成新的交付文档，例如：

```text
给XXX说的话.md
SC0479_MCU端交接文档.md
README_运行说明.md
README_协议对齐说明.md
```

必须使用：

```text
UTF-8 无 BOM
CRLF
```

Python 写法：

```python
from pathlib import Path

Path('给XXX说的话.md').write_text(
    content,
    encoding='utf-8',
    newline='\r\n'
)
```

生成后建议验证：

```python
data = Path('给CODEX说的话.md').read_bytes()
assert not data.startswith(b'\xef\xbb\xbf')  # 确认无 BOM
data.decode('utf-8')                         # 确认 UTF-8 可解码
```

---

## 10. Windows 路径处理规则

本项目路径包含中文和空格，例如：

```text
D:\BaiduSyncdisk\SC项目\SC-0479 4G网口开关盒子控制板 广州博宬 20260606
```

在脚本中必须使用原始字符串或 Path：

```python
from pathlib import Path

root = Path(r'D:\BaiduSyncdisk\SC项目\SC-0479 4G网口开关盒子控制板 广州博宬 20260606')
```

不要手写未转义路径：

```python
# 错误示例
root = "D:\BaiduSyncdisk\SC项目\..."
```

因为 `\S`、`\0` 等可能引发转义问题。

---

## 11. PowerShell 中文注意事项

在 PowerShell 中查看中文文件，建议：

```powershell
Get-Content -Encoding UTF8 .\给XXX说的话.md
```

如果是 GBK 文件：

```powershell
Get-Content -Encoding Default .\旧文件.c
```

PowerShell 7 可用：

```powershell
Get-Content -Encoding utf8
Get-Content -Encoding ansi
```

不要用不带编码参数的命令去判断文件是否乱码。

---

## 12. Git diff 前必须确认中文正常

修改中文文件后，必须检查：

```text
1. diff 中中文是否正常；
2. 是否出现大量无关乱码变更；
3. 是否出现全文件换行符变化；
4. 是否出现整文件重写。
```

如果 diff 显示整个文件都变化了，必须停止并排查：

```text
可能原因：
1. 编码被改了；
2. 换行符被改了；
3. 格式化工具重排了；
4. 读取时乱码再写回。
```

---

## 13. 小程序项目推荐目录

创建工程位置：

```text

```

新增文件全部 UTF-8：

```text
common/constants.js
common/protocol.js
common/mqttClient.js
common/statusMapper.js
pages/index/index.vue
pages/control/control.vue
pages/policy/policy.vue
pages/logs/logs.vue
pages/settings/settings.vue
README_运行说明.md
README_协议对齐说明.md
README_已实现和待办.md
```

---

## 14. 本项目的编码红线

以下情况必须立即停止：

```text
1. 中文显示为乱码；
2. diff 中中文大面积变化；
3. C 文件整文件被重写；
4. include_lib 文件出现大面积变更；
5. .cbp 工程文件结构被破坏；
6. 文档变成 ANSI 或 GBK；
7. uni-app 文件中文乱码。
```

处理方式：

```text
先回退当前文件；
重新检测编码；
只做最小必要修改；
修改后再次检查 diff。
```

---

## 15. 执行要求

每次修改前，Codex 必须说明：

```text
1. 准备修改哪些文件；
2. 这些文件当前识别到的编码；
3. 是否会保持原编码；
4. 是否会新增 UTF-8 文件；
5. 是否会影响 MCU 工程。
```

每次修改后，Codex 必须说明：

```text
1. 修改了哪些文件；
2. 是否保持原编码；
3. 是否检查了中文；
4. 是否检查了 diff；
5. 是否存在风险。
```

---

## 16. 简短版口诀

```text
先识别编码，再读取；
读出乱码，立即停止；
旧文件保持原编码；
新文件统一 UTF-8；
只改目标文件；
不要批量转码；
不要让乱码写回去。
```
