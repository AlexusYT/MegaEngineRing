#  // MegaEngineRing is a program that can speed up game development.
#  // Copyright (C) 2025. Timofeev (Alexus_XX) Alexander
#  //
#  // This program is free software; you can redistribute it and/or modify
#  // it under the terms of the GNU General Public License as published by
#  // the Free Software Foundation; either version 2 of the License, or
#  // (at your option) any later version.
#  //
#  // This program is distributed in the hope that it will be useful,
#  // but WITHOUT ANY WARRANTY; without even the implied warranty of
#  // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  // GNU General Public License for more details.
#  //
#  // You should have received a copy of the GNU General Public License along
#  // with this program; if not, write to the Free Software Foundation, Inc.,
#  // 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

import gdb.printing

import glm_pp

gdb.printing.register_pretty_printer(gdb.current_objfile(), glm_pp.GlmPP())

#
# def PrettyPrinter(arg):
#     name = getattr(arg, '__name__', arg)
#
#     def PrettyPrinterWrapperWrapperWrapper(func):
#
#         class PrettyPrinterWrapperWrapper:
#
#             class PrettyPrinterWrapper:
#                 def __init__(self, prefix, val, cb):
#                     self.prefix = prefix
#                     self.val = val
#                     self.cb = cb
#
#                 def to_string(self):
#                     return self.prefix + self.cb(self.val)
#
#             def __init__(self, name, cb):
#                 self.name = name
#                 self.enabled = True
#                 self.cb = cb
#
#             def __call__(self, val):
#                 prefix = ''
#                 if val.type.code == gdb.TYPE_CODE_PTR:
#                     prefix = '({}) {:#08x} '.format(str(val.type), int(val))
#                     try:
#                         val = val.dereference()
#                     except:
#                         return None
#                 valtype = val.type.unqualified()
#                 if valtype.name == self.name:
#                     return self.PrettyPrinterWrapper(prefix, val, self.cb)
#                 if valtype.strip_typedefs().name == self.name:
#                     return self.PrettyPrinterWrapper(prefix, val, self.cb)
#                 return None
#
#         pp = PrettyPrinterWrapperWrapper(name, func)
#         gdb.printing.register_pretty_printer(None, pp, True)
#
#     if callable(arg):
#         return PrettyPrinterWrapperWrapperWrapper(arg)
#
#     return PrettyPrinterWrapperWrapperWrapper
#
#
# def getElectricPrefix(value):
#     prefix = ""
#     prefixId = 0
#     temp = value
#     while abs(temp) > 999:
#         prefixId += 1
#         temp /= 1000.0
#     if prefixId == 0:
#         prefix = "u"
#     elif prefixId == 1:
#         prefix = "m"
#     elif prefixId == 2:
#         prefix = ""
#     elif prefixId == 3:
#         prefix = "k"
#     elif prefixId == 4:
#         prefix = "M"
#     elif prefixId == 5:
#         prefix = "G"
#     return [prefix, str(round(float(temp), 3))]
#
#
# @PrettyPrinter
# def Resistance(val):
#     prefix = getElectricPrefix(val["value"])
#     return prefix[1] + prefix[0] + "Ohm"
#
#
# @PrettyPrinter
# def Current(val):
#     prefix = getElectricPrefix(val["value"])
#     return prefix[1] + prefix[0] + "A"
#
#
# @PrettyPrinter
# def EMF(val):
#     prefix = getElectricPrefix(val["value"])
#     return prefix[1] + prefix[0] + "V"
#
#
# @PrettyPrinter
# def Potential(val):
#     prefix = getElectricPrefix(val["value"])
#     return prefix[1] + prefix[0] + "V"
#
#
# @PrettyPrinter
# def Resistivity(val):
#     prefix = getElectricPrefix(val["value"])
#     return prefix[1] + prefix[0] + "Ohm*mm2/m"
#
#
# @PrettyPrinter
# def Temperature(val):
#     prefix = ""
#     if val["scale"] == 0:
#         prefix = "K"
#     elif val["scale"] == 1:
#         prefix = "C"
#     elif val["scale"] == 2:
#         prefix = "F"
#     return val["value"] + prefix
#
#
# @PrettyPrinter
# def RequestData(val):
#     arr = []
#     state = val
#     if state == 0:
#         arr.append("INVISIBLE")
#     if state & 0b000001:
#         arr.append("VISIBLE")
#     if state & 0b000010:
#         arr.append("RESET_BUFFERS")
#     if state & 0b000100:
#         arr.append("RESET_MODEL_VIEW")
#     if state & 0b001000:
#         arr.append("RESET_SHADER")
#     if state & 0b010000:
#         arr.append("RESET_TEMP_UNIFORMS")
#     if state & 0b100000:
#         arr.append("RESET_GLOB_UNIFORMS")
#
#     result = ""
#     delim = ""
#     for str in arr:
#         result += delim + str
#         delim = ", "
#     return "{"+result+"}"
