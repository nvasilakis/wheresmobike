#!/usr/bin/env python

from lxml import etree, html
from urllib import urlencode
from urllib2 import Request, urlopen

CRAIG_ADDR = "http://www.craigslist.org/about/sites"
craig_response = urlopen(CRAIG_ADDR)
craig_root = html.parse(craig_response)

us = craig_root.xpath('//div[@class="colmask" and h1/a[@name="US"]]')
if us != []:
    cities = us[0].xpath('.//li/a/@href')
    for city in cities:
        addr = city + "/bia"
        city_response = urlopen(addr)
        city_root = html.parse(city_response)
        rows = city_root.xpath('//p[@class="row"]')

        for row in rows[1:2]:

            url = row.xpath('a/@href')[0]
            row_response = urlopen(url)
            row_root = html.parse(row_response)
            body = row_root.xpath('//section[@id="postingbody"]')
            imgUrl = row_root.xpath('//img[@id="iwi"]/@src')
            date = row_root.xpath('//date')

            # Do something with them
            if body != []:
                print body[0].text
            if imgUrl != []:
                print imgUrl[0]
            if date != []:
                print date[0].text

