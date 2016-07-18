import sys
import subprocess
import time
import tempfile

def instantiate_tempfile(in_fname, s):
    temp = tempfile.NamedTemporaryFile()
    with open(in_fname, 'r') as r:
        for line in r:
            temp.write(line.format(s))
    temp.flush()
    return temp


if __name__ == '__main__':

    dbname = 'bdtaunuhad_lite'

    job_suffixes = [
        'sp1235',
        'sp1237',
        ]

    executable_path = '../extract_mcbtypes'
    executable_cfg_template = 'extract_mcbtypes_python_template.cfg'
    populate_sql_script_template = 'populate_bmctypes_python_template.sql'

    for suffix in job_suffixes:

        print "+ begin processing {0}\n".format(suffix)
        start_all = time.time()

        print "  extracting bmctypes..."
        sys.stdout.flush()

        start = time.time()
        temp = instantiate_tempfile(executable_cfg_template, suffix)
        subprocess.check_call([executable_path, temp.name])
        temp.close()
        end = time.time()

        print "  completed in {0} seconds. \n".format(round(end-start, 2))
        sys.stdout.flush()

        print "  populating database..."
        sys.stdout.flush()

        start = time.time()
        temp = instantiate_tempfile(populate_sql_script_template, suffix)
        subprocess.check_call(["psql", "-d", dbname, "-f", temp.name])
        end = time.time()

        print "  completed in {0} seconds. \n".format(round(end-start, 2))
        sys.stdout.flush()

        print "  done. total runtime: {0} seconds \n".format(
                round(end-start_all), 2)

