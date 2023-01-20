from waflib.Configure import conf
from waflib.TaskGen import re_m4


def manpage_subst_fun(self, code):
    # Since we are providing a custom substitution method, we must implement
    # the default behavior, since we want that too.
    global re_m4

    # replace all % by %% to prevent errors by % signs
    code = code.replace('%', '%%')

    # extract the vars foo into lst and replace @foo@ by %(foo)s
    lst = []

    def repl(match):
        g = match.group
        if g(1):
            lst.append(g(1))
            return "%%(%s)s" % g(1)
        return ''
    code = getattr(self.generator, 're_m4', re_m4).sub(repl, code)

    try:
        d = self.generator.dct
    except AttributeError:
        d = {}
        for x in lst:
            tmp = getattr(self.generator, x, '') or self.env[x] or self.env[x.upper()]
            try:
                tmp = ''.join(tmp)
            except TypeError:
                tmp = str(tmp)
            d[x] = tmp

    code = code % d

    return code.replace("include::../docs/", "include::../../../docs/")


@conf
def manpage(ctx, section, source):

    if not ctx.env.BUILD_MAN:
        return

    ctx(features="subst",
        source=source,
        target=source + '.man-tmp',
        subst_fun=manpage_subst_fun)

    ctx(source=source + '.man-tmp', section=section)


@conf
def ntp_test(ctx, **kwargs):
    bldnode = ctx.bldnode.abspath()
    tg = ctx(**kwargs)

    args = ["%s/tests/%s" % (bldnode, tg.target), "-v"]

    if hasattr(tg, "test_args"):
        args += tg.test_args

    tg.ut_exec = args
