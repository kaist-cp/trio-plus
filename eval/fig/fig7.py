import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os


# 실험 파라미터
workloads = [
    'DWTL', 'MRPL', 'MRPM', 'MRPH',
    'MRDL', 'MRDM', 'MWCL', 'MWCM',
    'MWUL', 'MWUM', 'MWRL', 'MWRM'
]
titles = [
    'DWTL', 'MRPL', 'MRPM', 'MRPH',
    'MRDL', 'MRDM', 'MWCL', 'MWCM',
    'MWUL', 'MWUM', 'MWRL', 'MWRM'
]

# 파일 시스템 및 스타일 정의
fs_list = ['ext4', 'pmfs', 'nova', 'winefs', 'odinfs', 'sufs', 'sufs-fix']

linestyles = {
    'ext4': None,
    'pmfs': None,
    'nova': None,
    'winefs': None,
    'odinfs': None,
    'sufs': 'dashed',
    'sufs-fix': None
}
colors = {
    'ext4': '#E69F00', 'pmfs': '#009E73', 'nova': '#0072B2', 'winefs': '#CC79A7', 
    'extr': 'x-', 'odinfs': '#F0E442', 'sufs': '#D55E00', 'sufs-fix': '#D55E00'
}

# 경로 패턴
data_paths = {
    'ext4': '../data/fxmark/20250411-others/pmem-local:{fs}:{wl}:bufferedio.dat',
    'pmfs': '../data/fxmark/20250411-others/pmem-local:{fs}:{wl}:bufferedio.dat',
    'nova': '../data/fxmark/20250411-others/pmem-local:{fs}:{wl}:bufferedio.dat',
    'winefs': '../data/fxmark/20250411-others/pmem-local:{fs}:{wl}:bufferedio.dat',
    'extr': '../data/fxmark/20250411-others/dm-stripe:ext4:{wl}:bufferedio.dat',
    'odinfs': '../data/fxmark/20250411-others/pm-array:{fs}:{wl}:bufferedio.dat',
    'sufs' : '../data/fxmark/20250411-sufs-plain/pm-char-array:sufs:{wl}:bufferedio.dat',
    'sufs-fix': '../data/fxmark/20250411-sufs-fix/pm-char-array:sufs:{wl}:bufferedio.dat',
}

# Figure 생성
fig, axes = plt.subplots(3, 4, figsize=(14, 7), sharex=False)
axes = axes.flatten()

for idx, (wl, title) in reversed(list(enumerate(zip(workloads, titles)))):
    ax = axes[idx]
    for fs in fs_list:
        path_template = data_paths[fs]
        filepath = path_template.format(fs=fs, wl=wl)

        if not os.path.exists(filepath):
            print(f"No data for {fs} {title}")
            if idx == 0:
                ax.plot([], [], label=fs, color=colors[fs], linestyle=linestyles[fs])
                ax.legend(loc="upper right", fontsize=8)
            continue
        try:
            df = pd.read_csv(filepath, delim_whitespace=True, header=None, comment="#")

            x = df[0].astype(int)
            y = df[1].astype(float) / 1_000_000  # Convert to ops/μs
        
            ax.plot(x, y, color=colors[fs], linestyle=linestyles[fs], label=fs)
        except Exception as e:
            print(f"Error loading {filepath}: {e}")

    ax.set_title(title, pad=5)
    #if idx % 4 == 0:
    ax.set_ylabel(r'ops/$\mu$s')
    #if idx >= 6:
    ax.set_xlabel('# Threads')
# 레이아웃 조정 및 저장
plt.tight_layout()
plt.savefig('fig7.pdf')
plt.show()