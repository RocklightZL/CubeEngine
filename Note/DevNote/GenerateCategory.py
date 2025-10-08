import re
import os
from collections import defaultdict

def extract_tagged_content(md_content):
    # 标签映射（英文->中文）
    tag_mapping = {
        "todo": "待办事项",
        "bug": "Bug记录",
        "progress": "进展",
        "idea": "灵感",
        "note": "笔记"
    }
    
    # 初始化数据结构
    tag_content = defaultdict(list)
    current_date = None
    current_tag = None
    current_lines = []
    
    lines = md_content.split('\n')
    
    for line in lines:
        # 检测日期标题 (如 "# 7.31")
        date_match = re.match(r'^#\s+(\d+\.\d+)', line)
        if date_match:
            if current_tag and current_lines:
                # 保存时移除标签行
                tag_content[current_tag].append((current_date, '\n'.join(current_lines)))
                current_lines = []
            current_date = date_match.group(1)
            current_tag = None
            continue
        
        # 检测标签 (如 "/todo")
        tag_match = re.match(r'^/(\w+)', line)
        if tag_match:
            if current_tag and current_lines:
                tag_content[current_tag].append((current_date, '\n'.join(current_lines)))
            current_tag = tag_match.group(1)
            current_lines = []  # 重置内容行（不包含标签行）
            continue
        
        # 收集当前标签内容（跳过空行）
        if current_tag and line.strip():
            current_lines.append(line)
    
    # 处理最后一个标签块
    if current_tag and current_lines:
        tag_content[current_tag].append((current_date, '\n'.join(current_lines)))
    
    return tag_content, tag_mapping

def generate_tag_files(tag_content, tag_mapping):
    # 创建统一的分类文件夹
    output_dir = "category"
    os.makedirs(output_dir, exist_ok=True)
    
    for tag, entries in tag_content.items():
        if not entries:
            continue
        
        # 获取中文标签名
        zh_tag = tag_mapping.get(tag, tag)
        
        # 生成文件名
        filename = f"{output_dir}/{zh_tag}.md"
        
        # 构建内容
        content = [
            f"此文档包含所有标记为 `{zh_tag}` 的内容，按日期排序：\n",
            "---\n"
        ]
        
        for date, entry in entries:
            content.append(f"# {date}\n")  # 一级标题
            content.append(entry)
            content.append("\n---")
        
        # 写入文件
        with open(filename, 'w', encoding='utf-8') as f:
            f.write('\n'.join(content))
        print(f"已生成文件: {filename}")

def main():
    input_file = "DevelopmentNote.md"
    
    try:
        with open(input_file, 'r', encoding='utf-8') as f:
            content = f.read()
    except FileNotFoundError:
        print(f"错误: 文件 {input_file} 不存在")
        return
    
    tag_content, tag_mapping = extract_tagged_content(content)
    generate_tag_files(tag_content, tag_mapping)

if __name__ == "__main__":
    main()