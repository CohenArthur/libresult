project('option', 'c',
  default_options: [
  'warning_level=3',
  'debug=true',
  'c_std=c11',
  'b_sanitize=address',
  ])

tests = [
  'test_result.c',
]

foreach test_src: tests
  bin = executable('bin' + test_src, 'tests/' + test_src)
  test('bin' + test_src, bin, args: [
    '--gtest_color=yes',
    ])
endforeach
