# -*- coding: utf-8 -*-
import json
import random
import urllib.request
from urllib import parse
import re

import requests
from bs4 import BeautifulSoup
from slackclient import SlackClient
from flask import Flask, request, make_response, render_template
from datetime import datetime

app = Flask(__name__)

slack_token = ""
slack_client_id = ""
slack_client_secret = ""
slack_verification = ""
sc = SlackClient(slack_token)


def _response_to_person(inputText, dateInText):
    if inputText == '채용':
        # URL 데이터를 가져올 사이트 url 입력
        url = "http://www.saramin.co.kr/zf_user/calendar?cal_cd=1&cal_dt=2018-12&scrap=&keyword=&cal_kind%5B%5D=start&up_cd%5B%5D=3"

        # URL 주소에 있는 HTML 코드를 soup에 저장합니다.
        soup = BeautifulSoup(urllib.request.urlopen(url).read(), "html.parser")

        now = datetime.now()

        contents = []
        keywords = []
        company_url = []
        pretext = str(dateInText) + '일 시작되는 채용 공고 (IT 분야)'

        today_info = str(now.year) + '-' + str(now.month) + '-' + str(dateInText)
        print(today_info)

        for get_table in soup.find_all("div", class_="public_recruit_list_wrap"):
            if today_info == get_table.get_text()[:10]:
                cnt = 0

                for get_list in get_table.find_all("table", class_="public_recruit_list"):
                    one_unit = ''
                    for val_s in get_list.find_all("td", class_="state"):
                        one_unit = one_unit + val_s.get_text() + ':'
                        cnt += 1
                    for val_c in get_list.find_all("td", class_="company"):
                        one_unit = one_unit + val_c.get_text() + ':'
                    for val_t in get_list.find_all("td", class_="title"):
                        one_unit = one_unit + val_t.get_text() + ':'
                        for val_url in val_t.find_all("a"):
                            company_url.append("http://www.saramin.co.kr" + val_url["href"])

                    split_unit = one_unit.split(':')
                    width = int(len(split_unit) / cnt)
                    for n in range(cnt):
                        keyword = {}

                        R = str(hex(random.randrange(0, 256)))[2:]
                        G = str(hex(random.randrange(0, 256)))[2:]
                        B = str(hex(random.randrange(0, 256)))[2:]

                        for i in range(width):
                            contents.append(split_unit[n + cnt * i])

                        keyword["color"] = '#' + R + G + B
                        keyword["title"] = contents[1]

                        headers = {'User-Agent': 'Mozilla/5.0'}

                        search_url = "www.jobplanet.co.kr/search/companies/" + contents[1]
                        search_url = "http://" + parse.quote(search_url)
                        req = urllib.request.Request(search_url, headers=headers)
                        response = urllib.request.urlopen(req).read()
                        search_soup = BeautifulSoup(response, "html.parser")

                        com_rating = ''
                        for get_company in search_soup.find_all("span", class_="gfvalue"):
                            com_rating = get_company.get_text()
                            break

                        for get_logo_box in search_soup.find_all("a", class_="llogo"):
                            for get_logo in get_logo_box.find_all("img"):
                                keyword["thumb_url"] = get_logo["src"]

                        keyword["text"] = contents[0] + ' | ' + contents[width - 1] + '\n `기업 평점 : ' + com_rating +'`'
                        keyword["title_link"] = company_url[n]

                        contents = []
                        keywords.append(keyword)

        return (pretext, keywords)


    elif inputText == '토익':
        # URL 데이터를 가져올 사이트 url 입력
        url = "http://appexam.ybmnet.co.kr/toeic/info/receipt_schedule.asp"

        # URL 주소에 있는 HTML 코드를 soup에 저장합니다.
        soup = BeautifulSoup(urllib.request.urlopen(url).read(), "html.parser")

        contents = []
        keywords = []
        keyword = {}
        cnt = 0
        for date in soup.find_all("td", class_="str"):
            val_date = date.get_text()

            R = str(hex(random.randrange(0, 256)))[2:]
            G = str(hex(random.randrange(0, 256)))[2:]
            B = str(hex(random.randrange(0, 256)))[2:]

            if len(val_date) == 21:
                keyword["title"] = '- 시험일:' + val_date
                cnt += 1
            elif len(val_date) == 17:
                keyword["text"] = '- 성적발표일:' + val_date
                cnt += 1

            keyword["color"] = '#' + R + G + B

            if cnt == 2:
                keywords.append(keyword)
                cnt = 0
                keyword = {}

        real_ans = '가장 가까운 토익 시험 일정'

        return (real_ans, keywords)

    elif inputText == '오픽':
        url = "http://m.opic.or.kr/opics/servlet/controller.opic.site.receipt.AnnualScheduleServlet?p_process=select-list-mobile&p_tab=opic"

        # URL 주소에 있는 HTML 코드를 soup에 저장합니다.
        soup = BeautifulSoup(urllib.request.urlopen(url).read(), "html.parser")

        now = datetime.now()

        test_list = []
        keywords = []
        keyword = {}

        month_twoDigit = dateInText if int(dateInText) > 9 else '0' + dateInText

        real_ans = str(month_twoDigit) + '월 OPIc 시험 일정'

        for date in soup.find_all("div", class_="tbType06"):
            month_data = date.get_text().replace('<br/>', '').replace('\n', '').replace('\t', '')

            cnt = 0
            if month_data[:2] == month_twoDigit:
                for i in range(len(month_data[2:])):
                    if month_data[i] == '정':
                        if cnt == 0:
                            cnt += 1
                            continue
                        else:
                            test_list.append(month_data[i + 6:i + 20].replace('\r', ''))
                            test_list.append(month_data[i + 20:i + 48].replace('\r', ''))
                            test_list.append(month_data[i + 48:i + 61].replace('\r', ''))

        # test_list = test_list[:30]

        for list in range(len(test_list)):

            R = str(hex(random.randrange(0, 256)))[2:]
            G = str(hex(random.randrange(0, 256)))[2:]
            B = str(hex(random.randrange(0, 256)))[2:]

            if list % 3 == 0:
                keyword["title"] = '- 시험일:' + test_list[list]
                keyword["text"] = '- 접수기간:' + test_list[list + 1] + '\n' + '- 성적발표일:' + test_list[list + 2]

                keyword["color"] = '#' + R + G + B

                keywords.append(keyword)

                keyword = {}

        return (real_ans, keywords)

    elif inputText == '퇴근':
        return "감사합니다! 저 먼저 퇴근할게요! 안뇽~"

    else:
        return "무슨 말인지 모르겠어요.\n다시 입력해주세요\n(채용, 토익, 오픽)"


# 이벤트 핸들하는 함수
def _event_handler(event_type, slack_event):
    print(slack_event["event"])

    if event_type == "app_mention":
        channel = slack_event["event"]["channel"]
        text = slack_event["event"]["text"]

        if '채용' in text:
            date_in_text = re.findall("\d+", text[12:])
            #print(date_in_text)

            if len(date_in_text) == 0:
                now = datetime.now()
                date_in_text.append(now.day)

            keywords = _response_to_person('채용', date_in_text[0])

            if type(keywords) is tuple:
                sc.api_call(
                    "chat.postMessage",
                    channel=channel,
                    text=keywords[0],  # pretext
                    attachments=keywords[1]  # dictionary list
                )
            else:
                sc.api_call(
                    "chat.postMessage",
                    channel=channel,
                    text=keywords
                )

            return make_response("App mention message has been sent", 200, )

        elif '토익' in text:
            now = datetime.now()
            keywords = _response_to_person('토익', now.date)

            if type(keywords) is tuple:
                sc.api_call(
                    "chat.postMessage",
                    channel=channel,
                    text=keywords[0],  # pretext
                    attachments=keywords[1]  # dictionary list
                )
            else:
                sc.api_call(
                    "chat.postMessage",
                    channel=channel,
                    text=keywords
                )

            return make_response("App mention message has been sent", 200, )

        elif '오픽' in text:
            date_in_text = re.findall("\d+", text[12:])

            if len(date_in_text) == 0:
                now = datetime.now()
                date_in_text.append(now.month)

            keywords = _response_to_person('오픽', date_in_text[0])

            if type(keywords) is tuple:
                sc.api_call(
                    "chat.postMessage",
                    channel=channel,
                    text=keywords[0],  # pretext
                    attachments=keywords[1]  # dictionary list
                )
            else:
                sc.api_call(
                    "chat.postMessage",
                    channel=channel,
                    text=keywords
                )

            return make_response("App mention message has been sent", 200, )

        elif '퇴근' in text:
            keywords = _response_to_person('퇴근', 0)
            sc.api_call(
                "chat.postMessage",
                channel=channel,
                text=keywords
            )

        else:
            keywords = _response_to_person('예외', 0)
            sc.api_call(
                "chat.postMessage",
                channel=channel,
                text=keywords
            )

            return make_response("App mention message has been sent", 200, )

    # ============= Event Type Not Found! ============= #
    # If the event_type does not have a handler
    message = "You have not added an event handler for the %s" % event_type
    # Return a helpful error message
    return make_response(message, 200, {"X-Slack-No-Retry": 1})


@app.route("/listening", methods=["GET", "POST"])
def hears():
    slack_event = json.loads(request.data)

    if "challenge" in slack_event:
        return make_response(slack_event["challenge"], 200, {"content_type":
                                                                 "application/json"
                                                             })

    if slack_verification != slack_event.get("token"):
        message = "Invalid Slack verification token: %s" % (slack_event["token"])
        make_response(message, 403, {"X-Slack-No-Retry": 1})

    if "event" in slack_event:
        event_type = slack_event["event"]["type"]
        return _event_handler(event_type, slack_event)

    # If our bot hears things that are not events we've subscribed to,
    # send a quirky but helpful error response
    return make_response("[NO EVENT IN SLACK REQUEST] These are not the droids\
                         you're looking for.", 404, {"X-Slack-No-Retry": 1})


@app.route("/", methods=["GET"])
def index():
    return "<h1>Server is ready.</h1>"


if __name__ == '__main__':
    app.run('0.0.0.0', port=5000)