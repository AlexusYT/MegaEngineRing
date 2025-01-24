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

_type_letters = {
    gdb.TYPE_CODE_FLT: "d",  # or "f"
    gdb.TYPE_CODE_INT: "i",
    gdb.TYPE_CODE_BOOL: "b"
}


def _vec_info(v):
    # vec contains either a union of structs or a struct of unions, depending on
    # configuration. gdb can't properly access the named members, and in some
    # cases the names are wrong.
    # It would be simple to cast to an array, similarly to how operator[] is
    # implemented, but values returned by functions called from gdb don't have
    # an address.
    # Instead, recursively find all fields of required type and sort by offset.
    T = v.type.template_argument(1)
    # letter = _type_letters.get(T.value_type, "t")
    items = {}

    def find(v, bitpos):
        t = v.type.strip_typedefs()
        if t.code in (gdb.TYPE_CODE_STRUCT, gdb.TYPE_CODE_UNION):
            for f in t.fields():
                if hasattr(f, "bitpos"):  # not static
                    find(v[f], bitpos + f.bitpos)
        elif t == T:
            items[bitpos] = v

    find(v, 0)
    items = [str(f) for k, f in sorted(items.items())]
    return items


class VecPrinter:
    def __init__(self, v):
        self.v = v
        self.length = self.v.type.template_argument(0)
        self.items = _vec_info(self.v)

    # def children(self):
    #     result = [("X", float(self.items[0]))]
    #
    #     if self.length >= 2: result.append(("Y", float(self.items[1])))
    #     if self.length >= 3: result.append(("Z", float(self.items[2])))
    #     if self.length >= 4: result.append(("W", float(self.items[3])))
    #
    #     return result

    def display_hint(self):
        return "string"

    def roundValToStr(self, string):
        if "-nan" in string:
            result = "-nan"
        elif "nan" in string:
            result = "nan"
        elif "false" in string:
            result = "false"
        elif "true" in string:
            result = "true"
        else:
            result = str(round(float(string), 3))
        return result

    def to_string(self):

        resultStr = "("
        if self.length >= 1: resultStr += "X: " + self.roundValToStr(self.items[0])
        if self.length >= 2: resultStr += ", Y: " + self.roundValToStr(self.items[1])
        if self.length >= 3: resultStr += ", Z: " + self.roundValToStr(self.items[2])
        if self.length >= 4: resultStr += ", W: " + self.roundValToStr(self.items[3])
        return resultStr + ")"


class MatPrinter:
    def __init__(self, v):
        self.v = v

    def to_string(self):
        V = self.v["value"]
        columns = []
        for i in range(V.type.range()[1] + 1):
            letter, length, items = _vec_info(V[i])
            columns.append("({})".format(", ".join(items)))
        return "{}mat{}x{}({})".format(
            letter, len(columns), length, ", ".join(columns))


def GlmPP():
    pp = gdb.printing.RegexpCollectionPrettyPrinter("glm_pp")
    pp.add_printer(
        "glm::vec", r"^glm::vec<[^<>]*>$", VecPrinter)
    pp.add_printer(
        "glm::mat", r"^glm::(detail::)?tmat\dx\d<[^<>]*>$", MatPrinter)
    return pp
