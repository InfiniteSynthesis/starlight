var CxxEmitter = function () {
  Emitter.call(this, 'cc', '  ');
};


CxxEmitter.prototype = Object.create(Emitter.prototype, {
  constructor: {value: CxxEmitter},

  emitPrologue: {value: function () {
    this.push([
      '#include "gtest/gtest.h"',
      '',
      '#include "layout/style.h"',
      '#include "layout/mock_layout_host.h"',
      '#include "layout/layout_node.h"',
      '',
      'namespace starlight {',
      'class $LayoutTest : public testing::Test {',
      'public:',
      ' $LayoutTest() {}',
      ' LayoutNode* host_root(){ return host_.body(); };',
      'private:',
      ' MockLayoutHost host_;',
      '};',
    ]);
    this.pushIndent();
    this.push('');
  }},

  emitTestPrologue: {value: function(name, expriments) {
    this.push(['TEST_F($LayoutTest, ' + name + ') {']);
    this.pushIndent();
    this.push('');
  }},

  emitTestTreePrologue: {value: function(nodeName) {
    this.push('LayoutNode* ' + nodeName + ' = new LayoutNode();');
  }},

  emitTestEpilogue: {value: function (experiments) {
    this.popIndent();
    this.push([
      '}',
      ''
    ]);
  }},

  emitEpilogue: {value: function (lines) {
    this.popIndent();
    this.push([
      '}',
      '',
    ]);
  }},

  // 属性设置
  SetStyles: {
    value: function (styleMaps, nodeName) {
      var keys = Object.keys(styleMaps);
      var styleName = nodeName + 'Style';
      if (keys.length > 0) {
        for(var i=0; i<keys.length; i++ ) {
          this.push(nodeName + '->SetStyle(' + '"' + keys[i] + '",' + '"' + styleMaps[keys[i]] + '");');
        }
      }
    }
  },

  // 添加成为子节点
  NodeInsertChild: {
    value: function(parentName, nodeName, index) {
      this.push(parentName + '->InsertChild(' + nodeName + ', -1);');
    }
  },

  // 挂载根节点，计算
  AddElementToBody: {
    value: function() {
      this.push([
        'host_root()->InsertChild(root, 0);',
        'host_root()->ReLayout(0, 0, 400, 600);',
        ''
      ]);
    }
  },

  NodeLayoutGetLeft: {
    value: function(nodeName) {
      return nodeName + '->offset_left()';
    }
  },

  NodeLayoutGetTop: {
    value: function(nodeName) {
      return nodeName + '->offset_top()';
    }
  },

  NodeLayoutGetWidth: {
    value: function (nodeName) {
      return nodeName + '->offset_width()'
    }
  },

  NodeLayoutGetHeight: {
    value: function (nodeName) {
      return nodeName + '->offset_height()'
    }
  },


  TestTarget:{
    value:function(name) {
      // 通过<div>环境的id去定位assert的位置；
      if (name && name.length !== 0) {
        this.push('// 测试用例id ' + name );
      }
    }
  },  

  AssertEqual:{
    value:function(v0, v1) {
      this.push('EXPECT_NEAR(' + v0 + ', ' + v1 + ', 1);');
    }
  },
})
