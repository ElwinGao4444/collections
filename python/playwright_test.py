import logging
import tkinter as tk
from time import sleep
from playwright.sync_api import sync_playwright, TimeoutError 

def response(response):
    logging.info(f'response: {response.url}')
    if response.url == 'https://channels.weixin.qq.com/cgi-bin/mmfinderassistant-bin/live/close_live':
        logging.info('live finished')
    # print('debug: ', response.text())

def run(code, playwright):
    browser = playwright.chromium.launch(headless=False)
    context = browser.new_context()
    page = context.new_page()
    page.on('response', response)

    # 登录 & 获取用户ID
    page.goto('https://channels.weixin.qq.com/platform')
    if page.url == 'https://channels.weixin.qq.com/login.html':
        logging.info(f'waiting for login: {code}')
        page.wait_for_url('https://channels.weixin.qq.com/platform', timeout=0)
        user_id = page.locator('.finder-uniq-id').inner_text()
        logging.info(f'waiting for live: {user_id}')
    
    # 进入直播监听
    page.goto('https://channels.weixin.qq.com/platform/live/liveBuild')
    while True:
        try:
            page.wait_for_function('selector => !!document.querySelector(selector)', arg='.live-result-header-title', timeout=3*1000)
        except TimeoutError as e:
            if page.url != 'https://channels.weixin.qq.com/platform/live/liveBuild':
                page.goto('https://channels.weixin.qq.com/platform/live/liveBuild')
        else:
            live_time = page.locator('.live-result-header-time').inner_text()
            logging.info(f'finish live: {live_time}')
            break
    
    # 直播结束，监听浏览器退出
    logging.info('close browser')
    page.remove_listener('response', response)
    browser.close()

def show(code):
    # 启动一个playwright浏览器监听数据
    with sync_playwright() as playwright:
        run(code, playwright)

if __name__ == '__main__':
    logging.basicConfig()
    logging.getLogger().setLevel(logging.DEBUG)

    # 启动登录GUI
    root = tk.Tk()
    root.geometry('300x200')
    root.title('my window')
    entry = tk.Entry(root, width=30)
    entry.pack()
    button = tk.Button(root, text="Submit", command=lambda: show(entry.get()))
    button.pack()
    tk.mainloop()