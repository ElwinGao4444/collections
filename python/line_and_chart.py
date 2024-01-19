#!/usr/bin/env python
# -*- coding=utf8 -*-
"""
# Author: Elwin.Gao
# Created Time : Tue Aug 22 13:28:14 2023
# File Name: modules/line_and_chart.py
# Description:
"""

import json
import numpy as np
import pandas as pd

from datetime import datetime

class FTable():
    def __init__(self, df = None, date_unit = None, tz_local = None, info = None):
        if df is not None and not df.empty and df.index.name != 'time':
            df = df.set_index('time')
            df.index = pd.to_datetime(df.index, unit = date_unit)
            if tz_local is not None:
                df.index = df.index.tz_localize(tz_local)
        self.df = df

    def join(self, other):
        dfl = self.to_df()
        dfr = other.to_df()
        self_tags = dfl.columns.drop('value')
        other_tags = dfr.columns.drop('value')
        dfl = dfl.groupby(['time'] + self_tags.to_list()).sum(numeric_only=True)
        dfr = dfr.groupby(['time'] + dfr.columns.drop('value').to_list()).sum(numeric_only=True)
        df = dfl.join(dfr, lsuffix='_l', rsuffix='_r')
        return df.reset_index().set_index('time')

    def __calculator__(self, other, op, fillna = None):
        if isinstance(other, int) or isinstance(other, float):
            df = self.to_df().copy()
            df['value'] = op(df['value'], other)
            return df
        df = self.join(other)
        if fillna is not None:
            df = df.fillna(fillna)
        df['value'] = op(df['value_l'], df['value_r'])
        df.drop(['value_l', 'value_r'], inplace = True, axis = 1)
        return df

    def add(self, other, fillna = 0):
        return FTable(self.__calculator__(other, pd.Series.__add__, fillna))
    def __add__(self, other):
        return self.add(other, None)
    def sub(self, other, fillna = 0):
        return FTable(self.__calculator__(other, pd.Series.__sub__, fillna))
    def __sub__(self, other):
        return self.sub(other, None)
    def mul(self, other, fillna = 1):
        return FTable(self.__calculator__(other, pd.Series.__mul__, fillna))
    def __mul__(self, other):
        return self.mul(other, None)
    def div(self, other, fillna = 1):
        return FTable(self.__calculator__(other, pd.Series.__truediv__, fillna))
    def __truediv__(self, other):
        return self.div(other, None)

    def simple_filter(self, filters):
        condition = True
        for key, list in filters.items():
            if key not in self.df.columns:
                return FTable(pd.DataFrame())
            cond = False
            for value in list:
                cond = (cond | (self.df[key] == value))
            condition = (condition & cond)
        if isinstance(condition, pd.Series):
            return FTable(self.df[condition])
        return self

    def get_filters(self):
        tags = self.df.drop('value', axis=1)
        filters = {col.name: col.drop_duplicates().to_list() for idx, col in tags.items()}
        return filters

    def read_json(self, json_data):
        self.df = pd.read_json(json_data)
        if not self.df.empty:
            self.df.set_index('time', inplace = True)
            self.df.index = pd.to_datetime(self.df.index, unit='s')
        return self

    def read_sql(sql, conn):
        self.df = pd.read_sql(sql, conn)
        self.df = df.set_index('time', inplace = True)
        return self

    def read_peewee(self, result):
        self.df = pd.DataFrame(result).set_index('time')
        return self

    def convert_to_chart(self, json_columns = True):
        if self.df.empty:
            return FChart(self.df)
        df = None
        if json_columns:
            col_line_name = self.df.drop('value', axis=1).apply(lambda x: json.dumps(x.to_dict(), sort_keys=True, ensure_ascii=False), axis=1)
        else:
            col_line_name = self.df.drop('value', axis=1).apply(lambda x: ';'.join(x.to_list()), axis=1)
        df = pd.DataFrame({'line_name':col_line_name, 'value': self.df['value']}).reset_index()
        df = df[['time', 'line_name', 'value']].groupby(['time', 'line_name']).sum().unstack()
        df.columns = df.columns.map(lambda x: x[1])
        return FChart(df)

    def __str__(self):
        return self.df.__str__()

    def to_string(self):
        return self.df.to_string()

    def index_format(self, format='%Y-%m-%d %H:%M:%S'):
        self.df.index = self.df.index.strftime(format)
        return self

    def to_df(self):
        return self.df

    def is_empty(self):
        return self.df.empty

    def to_json(self, name = None, date_unit = 's', with_index = True):
        target = self.df
        if name is not None:
            target = target[name]
        if not with_index:
            target = target.reset_index()
        return target.to_json(date_unit = date_unit, force_ascii=False)

class FLine():
    def __init__(self, series = None, info = None):
        if series is None:
            self.series = pd.Series()
        else:
            self.series = series

    def __calculator__(self, other, op):
        if isinstance(other, FChart):
            if other.df.columns.size > 1:
                return FChart(other.df.apply(lambda x: op(self.to_series(), x)))
            else:
                other = other.get_line(other.df.columns[0])
        if isinstance(other, FLine):
            new_line = op(self.to_series(), other.to_series())
            new_line.name = '{"calculate":"%s"}'%(op.__name__)
            return FLine(new_line)
        return FLine(op(self.to_series(), other))

    def __add__(self, other):
        return self.__calculator__(other, pd.Series.__add__)
    def __sub__(self, other):
        return self.__calculator__(other, pd.Series.__sub__)
    def __mul__(self, other):
        return self.__calculator__(other, pd.Series.__mul__)
    def __truediv__(self, other):
        return self.__calculator__(other, pd.Series.__truediv__)

    def sum(self):
        return self.series.sum().item()
    def mean(self):
        return self.series.mean().item()

    def pack_to_chart(self):
        return FChart(self.series.to_frame())

    def __str__(self):
        return self.series.__str__()

    def to_string(self):
        return self.df.to_string()

    def to_series(self):
        return self.series

class FChart():
    def __init__(self, df = None, date_unit = None, tz_local = None, info = None):
        if df is not None and not df.empty and df.index.name != 'time':
            df = df.set_index('time')
            df.index = pd.to_datetime(df.index, unit=date_unit)
            if tz_local is not None:
                df.index = df.index.tz_localize(tz_local)
        self.df = df
    
    def __calculator__(self, other, op):
        if self.df.columns.size == 1:
            return getattr(self.get_line(self.df.columns[0]), op)(other).pack_to_chart()
        if isinstance(other, FChart) and other.df.columns.size == 1:
            other = other.get_line(other.df.columns[0])
        if isinstance(other, FChart):
            if self.df.columns.size == other.df.columns.size:
                return FChart(getattr(self.df, op)(other.df))
            else:
                return (getattr(self.convert_to_table(), op)(other.convert_to_table())).convert_to_chart()
        elif isinstance(other, FLine):
            return FChart(self.df.apply(lambda x: getattr(x, op)(other.series)))
        else:
            return FChart(getattr(self.df, op)(other))

    def __add__(self, other):
        return self.__calculator__(other, '__add__')
    def __sub__(self, other):
        return self.__calculator__(other, '__sub__')
    def __mul__(self, other):
        return self.__calculator__(other, '__mul__')
    def __truediv__(self, other):
        return self.__calculator__(other, '__truediv__')

    def sum(self, name = '{"calculate":"sum"}'):
        series = self.df.sum(axis=1)
        series.name = name
        return FLine(series).pack_to_chart()
    def mean(self, name = '{"calculate":"mean"}'):
        series = self.df.mean(axis=1)
        series.name = name
        return FLine(series).pack_to_chart()

    def cumsum(self):
        self.df = self.df.cumsum()
        return self

    def append_lines(self,fline):
        self.df = pd.concat([self.df, fline.to_df()], axis=1)
        return self

    def extend(self, other):
        self.df = pd.concat([self.df, other.df], axis=0)
        return self

    def merge_or_add(self, other):
        intersection = list(set(self.df.columns) & set(other.df.columns))
        difference = list(set(other.df.columns) - set(self.df.columns))
        if intersection:
            self.df = self.df.add(other.df[intersection], fill_value = 0)
        if difference:
            self.df = self.df.join(other.df[difference])
        return self

    def merge(self, other):
        self.df = self.df.join(other.df)
        return self

    def shift(self, periods = 1, freq = 'D'):
        self.df = self.df.shift(periods, freq)
        return self

    def rolling(self, window = 7):
        self.df = self.df.rolling(window).mean()
        return self

    def rename(self, name):
        self.df.rename({self.df.columns[0]:name}, inplace=True, axis=1)
        return self

    def add_attr(self, key, value = 'NULL'):
        self.df.columns = self.df.columns.map(lambda x: x[:-1]+',"%s":"%s"}'%(key,value) if x != '{}' else '{"%s":"%s"}'%(key,value))
        return self

    def get_line(self, name):
        return FLine(self.df[name])

    def simple_filter(self, filters):
        if self.df.columns.map(lambda x: x.startswith('{')).all():
            return self.convert_to_table().simple_filter(filters).convert_to_chart()
        return self

    def get_filters(self):
        if self.df.columns.map(lambda x: x.startswith('{')).all():
            return self.convert_to_table().get_filters()
        return {}

    def group_by_granularity(self, granularity, fun):
        if granularity == "month":
            return self.resample('MS', fun)
        elif granularity == "week":
            return self.resample('7D', fun)
        elif granularity == "range":
            return self.resample(str(len(self.df)) + 'D', fun)
        elif granularity == "day":
            pass
        else:
            return self.resample(str(granularity) + 'D', fun)
        return self

    def convert_to_table(self):
        series = self.df.stack()
        series.index.rename(['time', 'line_name'], inplace = True)
        series.rename('value', inplace = True)
        df = series.to_frame()
        df.reset_index(level=[1], inplace = True)
        tag_df = df['line_name'].apply(lambda x: pd.Series(json.loads(x)))
        df = pd.concat([tag_df, df.drop('line_name', axis=1)], axis = 1)
        return FTable(df)

    def simplify_json_columns(self):
        if self.df.empty:
            return self
        self.df.columns = self.df.columns.map(lambda x: ';'.join(json.loads(x).values()))
        return self

    def is_empty(self):
        return self.df.empty

    def complete_datetime(self, start, end, periods=None, freq=None, tz=None):
        self.df = self.df.reindex(pd.date_range(start, end, periods, freq, tz))
        self.df.index.name = 'time'
        return self

    def time_interval(self):
        if self.df.shape[0] < 2:
            return None
        diff = self.df.index[1] - self.df.index[0]
        return diff.seconds

    def cut_off(self, start, end):
        self.df = self.df.loc[start : end]
        return self

    def resample(self, rule, fun = 'mean'):
        if self.df.empty:
            return self
        self.df = getattr(self.df.resample(rule), fun)()
        return self

    def pack_to_chart(self):
        return self

    def __getitem__(self, item):
        series = self.df['item']
        return FLine(series)

    def __str__(self):
        return self.df.__str__()

    def to_string(self):
        return self.df.to_string()

    def dropna(self, how = 'all'):
        self.df.dropna(axis=1, how = how, inplace = True)
        return self

    def tz_localize(self, tz_local = 'UTC'):
        self.df.index = self.df.index.tz_localize(tz_local)
        return self

    def tz_convert(self, tz_convert = 'Asia/Shanghai'):
        self.df.index = self.df.index.tz_convert(tz_convert)
        return self

    def to_df(self, name = None):
        if name is not None:
            return self.df[name]
        return self.df

    def index_format(self, format='%Y-%m-%d %H:%M:%S'):
        if self.df.empty:
            return self
        self.df.index = self.df.index.strftime(format)
        return self

    def read_json(self, json_data, unit='s'):
        self.df = pd.read_json(json_data).astype(float) 
        if not self.df.empty:
            self.df.set_index('time', inplace = True)
            self.df.index = pd.to_datetime(self.df.index, unit=unit)
        return self

    def to_json(self, name = None, date_unit = 's', with_index = True):
        target = self.df
        if name is not None:
            target = target[name]
        if not with_index:
            target = target.reset_index()
        return target.to_json(date_unit = date_unit, force_ascii=False)

############################################ Unit Test ############################################

import unittest

base_df = pd.DataFrame(data = {
    'value': np.array([1,2,3])
}, dtype=float, index = pd.Index(pd.date_range(datetime(2023,8,1,0,0,0), periods=3, freq='s'), name='time'))
base_df = pd.concat([base_df]*3)
base_df['a'] = ['a1']*3 + ['a2']*3 + ['a3']*3
base_df['b'] = ['b1']*3 + ['b2']*3 + ['b3']*3
base_df = pd.concat([base_df]*3)
base_df['c'] = ['c1']*9 + ['c2']*9 + ['c3']*9
base_df.sort_index()

df1 = base_df[['a','b','value']].copy()
df1 = df1.groupby(['time', 'a', 'b']).sum(numeric_only=True)
df2 = base_df.copy()
df2 = df2.groupby(['time', 'a', 'b', 'c']).sum(numeric_only=True)

table = FTable(df2.copy().reset_index())
chart = FChart(table.convert_to_chart().to_df().reset_index())
line = FLine(chart.to_df()['{"a": "a1", "b": "b1", "c": "c1"}'])

@unittest.skip("skipped test")
class TestLine(unittest.TestCase):
    def setUp(self):
        self.line = line
        print('start testing: TestLine', end='')
    def tearDown(self):
        print('end testing: TestLine')

    # @unittest.skip("skipped test")
    def test_arithmetic_operation(self):
        print('[test_arithmetic_operation]')
        print(self.line)
        print(self.line + self.line)
        print(self.line - self.line)
        print(self.line * self.line)
        print(self.line / self.line)
        print(self.line + 10)
        print(self.line - 10)
        print(self.line * 10)
        print(self.line / 10)

    # @unittest.skip("skipped test")
    def test_functional_operation(self):
        print('[test_functional_operation]')
        print(self.line.sum())
        print(self.line.mean())

    # @unittest.skip("skipped test")
    def test_conversion(self):
        print('[test_conversion]')
        print(self.line.pack_to_chart())

@unittest.skip("skipped test")
class TestTable(unittest.TestCase):
    def setUp(self):
        self.table = table
        self.t1 = FTable(df1.copy().reset_index())
        self.t2 = FTable(df2.copy().reset_index())
        print('start testing: TestTable', end='')
    def tearDown(self):
        print('end testing: TestTable')

    # @unittest.skip("skipped test")
    def test_arithmetic_operation(self):
        print('[test_arithmetic_operation]')
        print(self.table)
        print(self.table + self.table)
        print(self.table - self.table)
        print(self.table * self.table)
        print(self.table / self.table)
        print(self.table + 10)
        print(self.table - 10)
        print(self.table * 10)
        print(self.table / 10)

    # @unittest.skip("skipped test")
    def test_arithmetic_operation_join(self):
        print('[test_arithmetic_operation_join]')
        print(self.t1)
        print(self.t2)
        print(self.t1 + self.t2)
        print(self.t1 - self.t2)
        print(self.t1 * self.t2)
        print(self.t1 / self.t2)

    # @unittest.skip("skipped test")
    def test_conversion(self):
        print('[test_conversion]')
        print(self.table.convert_to_chart())

# @unittest.skip("skipped test")
class TestChart(unittest.TestCase):
    def setUp(self):
        self.chart = chart
        self.line = line
        self.c1 = FTable(df1.copy().reset_index()).convert_to_chart()
        self.c2 = FTable(df2.copy().reset_index()).convert_to_chart()
        print('start testing: TestChart', end='')
    def tearDown(self):
        print('end testing: TestChart')

    # @unittest.skip("skipped test")
    def test_arithmetic_operation(self):
        print('[test_arithmetic_operation]')
        print(self.chart)
        print(self.chart + self.chart)
        print(self.chart - self.chart)
        print(self.chart * self.chart)
        print(self.chart / self.chart)
        print(self.chart + self.line)
        print(self.chart - self.line)
        print(self.chart * self.line)
        print(self.chart / self.line)
        print(self.chart + 10)
        print(self.chart - 10)
        print(self.chart * 10)
        print(self.chart / 10)

    # @unittest.skip("skipped test")
    def test_arithmetic_operation_join(self):
        print('[test_arithmetic_operation_join]')
        print(self.c1)
        print(self.c2)
        print(self.c1 + self.c2)
        print(self.c1 - self.c2)
        print(self.c1 * self.c2)
        print(self.c1 / self.c2)

    # @unittest.skip("skipped test")
    def test_functional_operation(self):
        print('[test_functional_operation]')
        print(self.chart.sum())
        print(self.chart.mean())

    # @unittest.skip("skipped test")
    def test_anonymous_operation(self):
        print('[test_anonymous_operation]')
        df = chart.df[['{"a": "a1", "b": "b1", "c": "c1"}']]
        df ['{"a": "a1", "b": "b1", "c": "c1"}'] = 100
        line = FChart(df)
        print(self.chart + line)
        print(self.chart - line)
        print(line - self.chart)
        print(line * line)
        print(line/1000)

    # @unittest.skip("skipped test")
    def test_conversion(self):
        print('[test_conversion]')
        print(self.chart.pack_to_chart())
        print(self.chart.convert_to_table())

    # @unittest.skip("skipped test")
    def test_filter(self):
        print('[test_filter]')
        print(self.chart.simple_filter({'a': ['a1', 'a2'], 'b': ['b1', 'b2']}))

if __name__ == '__main__':
    unittest.main()

    # # test read from peewee
    # from modules.timeseries import TimeKeyValueTable as dbtable
    # test_data = [
    #         {'type': 'test', 'time': datetime(2023,8,1,0,0,0), 'key': 'k1', 'value': 100},
    #         {'type': 'test', 'time': datetime(2023,8,1,0,0,1), 'key': 'k2', 'value': 200},
    #         {'type': 'test', 'time': datetime(2023,8,1,0,0,2), 'key': 'k3', 'value': 300},
    #         ]
    # dbtable.insert_many(test_data).on_conflict_replace().execute()
    # result = dbtable.select(dbtable.type, dbtable.time, dbtable.key, dbtable.value).where(dbtable.type == 'test').dicts()
    # t = FTable()
    # t.read_peewee(result)
    # print('test: \n', t)

